#include "TextureAnalysis.h"
#include "RWImage.h"
#include <opencv2/opencv.hpp>
#include <opencv2/viz.hpp>
#include <opencv2/ml.hpp>
#include <limits>

using namespace cv;

TextureAnalysis::TextureAnalysis(std::string path, int h, int w, int b):height(h), width(w), BytesPerPixel(b){
    RWImage rwi(h, w);
    for(int i = 1; i <= 9; i++){
        train_image.push_back(rwi.readRawImage(path + "train/blanket_" + std::to_string(i) + ".raw", b));
        train_image.push_back(rwi.readRawImage(path + "train/brick_" + std::to_string(i) + ".raw", b));
        train_image.push_back(rwi.readRawImage(path + "train/grass_" + std::to_string(i) + ".raw", b));
        train_image.push_back(rwi.readRawImage(path + "train/stones_" + std::to_string(i) + ".raw", b));
        train_labels.push_back(1);
        train_labels.push_back(2);
        train_labels.push_back(3);
        train_labels.push_back(4);
    }
    for(int i = 1; i <= 12; i++){
        test_image.push_back(rwi.readRawImage(path + "test/" + std::to_string(i) + ".raw", b));
    }
    test_labels.push_back(2);
    test_labels.push_back(1);
    test_labels.push_back(4);
    test_labels.push_back(1);
    test_labels.push_back(4);
    test_labels.push_back(3);
    test_labels.push_back(3);
    test_labels.push_back(4);
    test_labels.push_back(2);
    test_labels.push_back(2);
    test_labels.push_back(1);
    test_labels.push_back(3);
}

// Function to calculate intra-variability
float calculateIntraVariability(const std::vector<Mat>& featureVectors, const std::vector<int>& labels, int dimension) {
    float sum = 0.0;
    int totalCount = 0;

    for(int j = 1; j <= 4; j++){
        float classSum = 0.0;
        int classCount = 0;
        for (int i = 0; i < featureVectors.size(); i++) {
            if (labels[i] == j) {
                classSum += featureVectors[i].at<float>(0, dimension);
                classCount++;
            }
        }

        float classMean = classSum / classCount;

        float classIntraVariability = 0.0;

        for (int i = 0; i < featureVectors.size(); ++i) {
            if (labels[i] == j) {
                float diff = featureVectors[i].at<float>(0, dimension) - classMean;
                classIntraVariability += diff * diff;
            }
        }

        sum += classIntraVariability;
        totalCount += classCount;
    }

    return sum / totalCount;
}

// Function to calculate inter-variability
float calculateInterVariability(const std::vector<Mat>& featureVectors, const std::vector<int>& labels, int dimension) {
    float sum = 0.0;
    int totalCount = 0;

    for(int j = 1; j <= 4; j++){
        float classSum = 0.0;
        int classCount = 0;
        for (int i = 0; i < featureVectors.size(); i++) {
            if (labels[i] != j) {
                classSum += featureVectors[i].at<float>(0, dimension);
                classCount++;
            }
        }

        float classMean = classSum / classCount;

        float classInterVariability = 0.0;

        for (int i = 0; i < featureVectors.size(); ++i) {
            if (labels[i] != j) {
                float diff = featureVectors[i].at<float>(0, dimension) - classMean;
                classInterVariability += diff * diff;
            }
        }

        sum += classInterVariability;
        totalCount += classCount;
    }

    return sum / totalCount;
}

// Function to compute class mean vectors and covariance matrices
void computeClassStatistics(const Mat& features, const std::vector<int>& labels, int numClasses, std::vector<Mat>& classMeanVectors, std::vector<Mat>& classCovarianceMatrices) {
    classMeanVectors.resize(numClasses);
    classCovarianceMatrices.resize(numClasses);

    

    for (int i = 0; i < numClasses; i++) {
        // Extract features of samples belonging to the current class
        std::vector<Mat> classSamples;
        for (int j = 0; j < features.rows; j++) {
            if (labels[j] == (i + 1)) {
                classSamples.push_back(features.row(j));
            }
        }

        // Compute mean vector
        Mat classSamplesMat;
        vconcat(classSamples, classSamplesMat);
        reduce(classSamplesMat, classMeanVectors[i], 0, REDUCE_AVG);

        // Compute covariance matrix
        Mat covariance;
        calcCovarMatrix(classSamplesMat, covariance, classMeanVectors[i], COVAR_NORMAL | COVAR_ROWS);       
        classCovarianceMatrices[i] = covariance;
    }
}

// Function to compute Mahalanobis Distance
float calculateMahalanobisDistance(const Mat& x, const Mat& mean, const Mat& covariance) {
    Mat mean32;
    mean.convertTo(mean32, CV_32F);
    Mat diff = x - mean32;
    Mat inverseCovariance,inverseCovariance32;  
    invert(covariance, inverseCovariance);
    inverseCovariance.convertTo(inverseCovariance32, CV_32F);
    Mat distance = diff * inverseCovariance32 * diff.t();
    float mahalanobisDist = sqrt(distance.at<float>(0, 0));
    return mahalanobisDist;
}

// Function to classify test images using nearest neighbor (Mahalanobis Distance)
std::vector<int> classifyImages(const Mat& testFeatures, const std::vector<Mat>& classMeanVectors, const std::vector<Mat>& classCovarianceMatrices) {
    std::vector<int> predictedLabels;

    for (int i = 0; i < testFeatures.rows; i++) {
        Mat testFeature = testFeatures.row(i);
        int predictedLabel = -1; // Initialize predicted label
        float minDistance = std::numeric_limits<float>::max(); // Initialize minimum distance to maximum value

        // Compute distance from test sample to each class and find the class with minimum distance as predicted label
        for (int j = 0; j < classMeanVectors.size(); ++j) {
            float distance = calculateMahalanobisDistance(testFeature, classMeanVectors[j], classCovarianceMatrices[j]);
            if (distance < minDistance) {
                minDistance = distance;
                predictedLabel = (j + 1); // Class index as predicted label
            }
        }
        predictedLabels.push_back(predictedLabel); // Store predicted result
    }

    return predictedLabels;
}

void calculateFeatureVectors(std::vector<Mat> &inputImage ,std::vector<Mat> &FeatureVectors, int width, int height){
    // create laws filters
    std::vector<Mat> lawFilters_1d;
    std::vector<Mat> lawFilters;

    Mat mat1(1, 5, CV_32F);
    mat1.at<float>(0,0) = 1;
    mat1.at<float>(0,1) = 4;
    mat1.at<float>(0,2) = 6;
    mat1.at<float>(0,3) = 4;
    mat1.at<float>(0,4) = 1;
    lawFilters_1d.push_back(mat1);

    Mat mat2(1, 5, CV_32F);
    mat2.at<float>(0,0) = -1;
    mat2.at<float>(0,1) = -2;
    mat2.at<float>(0,2) = 0;
    mat2.at<float>(0,3) = 2;
    mat2.at<float>(0,4) = 1;
    lawFilters_1d.push_back(mat2);

    Mat mat3(1, 5, CV_32F);     
    mat3.at<float>(0,0) = -1;
    mat3.at<float>(0,1) = 0;
    mat3.at<float>(0,2) = 2;
    mat3.at<float>(0,3) = 0;
    mat3.at<float>(0,4) = -1;
    lawFilters_1d.push_back(mat3);

    Mat mat4(1, 5, CV_32F);
    mat4.at<float>(0,0) = -1;
    mat4.at<float>(0,1) = 2;
    mat4.at<float>(0,2) = 0;
    mat4.at<float>(0,3) = -2;
    mat4.at<float>(0,4) = 1;
    lawFilters_1d.push_back(mat4);

    Mat mat5(1, 5, CV_32F);
    mat5.at<float>(0,0) = 1;
    mat5.at<float>(0,1) = -4;
    mat5.at<float>(0,2) = 6;
    mat5.at<float>(0,3) = -4;
    mat5.at<float>(0,4) = 1;
    lawFilters_1d.push_back(mat5);

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            lawFilters.push_back(lawFilters_1d[i].t() * lawFilters_1d[j]);
        }
    }

    // calculate train feature vectors
   

    for(int i = 0; i < inputImage.size(); i++){
        Mat image = inputImage[i];

        // Subtract image mean
        Scalar meanVal = mean(image);
        image -= meanVal;

        Mat temp(1,25,CV_32F);

        // Apply 25 5x5 Laws filters
        for(int j = 0; j < 25; j++){
            Mat filteredImage;
            filter2D(image, filteredImage, -1, lawFilters[j],Point(-1,-1),0,BORDER_REFLECT);

            // Compute average energy
            Scalar energy = sum(filteredImage.mul(filteredImage)) / (width * height);

            temp.at<float>(0,j) = sqrt(static_cast<float>(energy.val[0]));
        }

        // store feature vector
        FeatureVectors.push_back(temp);
    }
}

void TextureAnalysis::calculateDiscriminantPower(std::vector<Mat> &FeatureVectors){
    // Calculate discriminant power
    std::vector<float> discriminantPowers;
    for(int i = 0; i < 25; i++){
        // Calculate intra-variability and inter-variability
        float intraVariability = calculateIntraVariability(FeatureVectors, train_labels, i);
        float interVariability = calculateInterVariability(FeatureVectors, train_labels, i);

        // Calculate discriminant power
        float discriminantPower = intraVariability / interVariability;

        discriminantPowers.push_back(discriminantPower);
    }

    std::cout << "dimension discriminantPower:" << std::endl;
    for(int i = 0; i < 25; i++){
        std::cout << "dimension" << i + 1 << ": " << discriminantPowers[i] << std::endl;
    }
}

void TextureAnalysis::nearestNeighborRuleClassifier(){

     std::vector<Mat> trainFeatureVectors;
     calculateFeatureVectors(train_image, trainFeatureVectors, width, height);

    // int n = 0;
    // for(Mat m : trainFeatureVectors){
    //     std::cout << "image" << n << ": " << m << std::endl;
    //     n++;
    // }

    // calculateDiscriminantPower(trainFeatureVectors);
    
    // pca for train data
    Mat train_data;
    for (const auto& featureVector : trainFeatureVectors) {
        train_data.push_back(featureVector);
    }

    Mat norm_train_data;
    normalize(train_data, norm_train_data, 0, 1, NORM_MINMAX);
    PCA train_pca(norm_train_data, Mat(), PCA::DATA_AS_ROW, 3);
    Mat reducedTrainFeatures = train_pca.project(norm_train_data);
    // std::cout << reducedTrainFeatures << std::endl;

    // Classifier: Nearest Neighbor
    // calculate test feature vectors
    std::vector<Mat> testFeatureVectors;
    calculateFeatureVectors(test_image, testFeatureVectors, width, height);

    // pca for test data
    Mat test_data;
    for (const auto& featureVector : testFeatureVectors) {
        test_data.push_back(featureVector);
    }

    Mat norm_test_data;
    normalize(test_data, norm_test_data, 0, 1, NORM_MINMAX);
    PCA test_pca(norm_test_data, Mat(), PCA::DATA_AS_ROW, 3);
    Mat reducedTestFeatures = test_pca.project(norm_test_data);

    // std::vector<Point3f> points3D;

    // // Iterate over each row of features3D, convert it to a Point3f object and add it to points3D
    // for (int i = 0; i < reducedTrainFeatures.rows; ++i) {
    //     Point3f point;
    //     point.x = reducedTrainFeatures.at<float>(i, 0) ; // Assume that the first element of the eigenvector is the x-coordinate
    //     point.y = reducedTrainFeatures.at<float>(i, 1) ; // Assume that the first element of the eigenvector is the y-coordinate
    //     point.z = reducedTrainFeatures.at<float>(i, 2) ; // Assume that the first element of the eigenvector is the z-coordinate
    //     points3D.push_back(point);
    // }

    // viz::Viz3d window("3D Feature Space");
    // window.showWidget("Coordinate Widget", viz::WCoordinateSystem());
    // // Create a point cloud object and add points
    // viz::WCloud cloudWidget(points3D, viz::Color::green());
    // cloudWidget.setRenderingProperty(viz::POINT_SIZE, 10.0);
    // window.showWidget("Cloud Widget", cloudWidget);

    // // Start the event loop until the user closes the window
    // while (!window.wasStopped()) {
    //     window.spinOnce(1, true);
    // }


    // calculate Mahalanobis Distance
    std::vector<Mat> classMeanVectors1;
    std::vector<Mat> classCovarianceMatrices1;
    computeClassStatistics(train_data, train_labels, 4, classMeanVectors1, classCovarianceMatrices1); 

    // Classify test images
    std::vector<int> predictedLabels1 = classifyImages(test_data, classMeanVectors1, classCovarianceMatrices1);

    std::cout << "25-D classify:" << std::endl;
    float errorNumber1 = 0;
    for(int i = 0; i < test_image.size(); i++){
        std::cout << predictedLabels1[i] << " ";
        if(predictedLabels1[i] != test_labels[i]){
            errorNumber1++;
        }
    }
    std::cout << std::endl;
    std::cout << "error rate :" << errorNumber1 / 12 << std::endl;

    std::vector<Mat> classMeanVectors2;
    std::vector<Mat> classCovarianceMatrices2;
    computeClassStatistics(reducedTrainFeatures, train_labels, 4, classMeanVectors2, classCovarianceMatrices2); 

    // Classify test images
    std::vector<int> predictedLabels2 = classifyImages(reducedTestFeatures, classMeanVectors2, classCovarianceMatrices2);

    std::cout << "3-D classify:" << std::endl;
    float errorNumber2 = 0;
    for(int i = 0; i < test_image.size(); i++){
        std::cout << predictedLabels2[i] << " ";
        if(predictedLabels2[i] != test_labels[i]){
            errorNumber2++;
        }
    }
    std::cout << std::endl;
    std::cout << "error rate :" << errorNumber2 / 12 << std::endl;

    // rwi.writeRawImage(outputPath, resultImage, 3);
     
}

void TextureAnalysis::K_meansClassifier(){

    // calculate train feature vectors
    std::vector<Mat> trainFeatureVectors;
    calculateFeatureVectors(train_image, trainFeatureVectors, width, height);

    // pca for train data
    Mat train_data;
    for (const auto& featureVector : trainFeatureVectors) {
        train_data.push_back(featureVector);
    }

    PCA train_pca(train_data, Mat(), PCA::DATA_AS_ROW, 3);
    Mat reducedTrainFeatures = train_pca.project(train_data);

    // calculate test feature vectors
    std::vector<Mat> testFeatureVectors;
    calculateFeatureVectors(test_image, testFeatureVectors, width, height);

    // pca for test data
    Mat test_data;
    for (const auto& featureVector : testFeatureVectors) {
        test_data.push_back(featureVector);
    }

    PCA test_pca(test_data, Mat(), PCA::DATA_AS_ROW, 3);
    Mat reducedTestFeatures = test_pca.project(test_data);

    // K-means Classification
    int K = 4;

    Mat trainlabels(train_labels.size(), 1, CV_32S);
    Mat labels25D(test_data.rows, 1, CV_32S);
    Mat centroids25D;
    TermCriteria criteria25D(TermCriteria::EPS + TermCriteria::COUNT, 100, 0.1);
    kmeans(train_data, K, trainlabels, criteria25D, 10, KMEANS_PP_CENTERS, centroids25D);
    kmeans(test_data, K, labels25D, criteria25D, 3, KMEANS_USE_INITIAL_LABELS, centroids25D);


    Mat labels3D(reducedTestFeatures.rows, 1, CV_32S);
    Mat centroids3D;
    TermCriteria criteria3D(TermCriteria::EPS + TermCriteria::COUNT, 100, 0.1);
    kmeans(reducedTrainFeatures, K, trainlabels, criteria3D, 3, KMEANS_RANDOM_CENTERS, centroids3D);
    kmeans(reducedTestFeatures, K, labels3D, criteria3D, 3, KMEANS_USE_INITIAL_LABELS, centroids3D);

    // calculate error rate
    float errorNumber = 0;
    std::cout << "25D classify:" << std::endl;
    for(int i = 0; i < labels25D.rows; i++){
        int c = labels25D.at<int>(i,0) + 1;
        std::cout << c << " ";
        if(c != test_labels[i]){
            errorNumber++;
        }
    }
    std::cout << std::endl;
    std::cout << "error rate :" << errorNumber / 12 << std::endl;

    errorNumber = 0;
    std::cout << "3D classify:" << std::endl;
    for(int i = 0; i < labels3D.rows; i++){
        int c = labels3D.at<int>(i,0) + 1;
        std::cout << c << " ";
        if(c != test_labels[i]){
            errorNumber++;
        }
    }
    std::cout << std::endl;
    std::cout << "error rate :" << errorNumber / 12 << std::endl;
}


void TextureAnalysis::SVMaClassifier(){

    // calculate train feature vectors
    std::vector<Mat> trainFeatureVectors;
    calculateFeatureVectors(train_image, trainFeatureVectors, width, height);

    // pca for train data
    Mat train_data;
    for (const auto& featureVector : trainFeatureVectors) {
        train_data.push_back(featureVector);
    }

    PCA train_pca(train_data, Mat(), PCA::DATA_AS_ROW, 3);
    Mat reducedTrainFeatures = train_pca.project(train_data);

    // calculate test feature vectors
    std::vector<Mat> testFeatureVectors;
    calculateFeatureVectors(test_image, testFeatureVectors, width, height);
    
    // pca for test data
    Mat test_data;
    for (const auto& featureVector : testFeatureVectors) {
        test_data.push_back(featureVector);
    }

    PCA test_pca(test_data, Mat(), PCA::DATA_AS_ROW, 3);
    Mat reducedTestFeatures = test_pca.project(test_data);

    // svm
    // Defining SVM parameters
    Ptr<ml::SVM> svm25D = ml::SVM::create();
    Ptr<ml::SVM> svm3D = ml::SVM::create();

    // Train SVM model (25-D features)
    svm25D->setType(ml::SVM::C_SVC);
    svm25D->setKernel(ml::SVM::LINEAR);
    svm25D->train(train_data, ml::ROW_SAMPLE, train_labels);

    // Train SVM model (3-D features)
    svm3D->setType(ml::SVM::C_SVC);
    svm3D->setKernel(ml::SVM::LINEAR);
    svm3D->train(reducedTrainFeatures, ml::ROW_SAMPLE, train_labels);

    // Predict test set labels (25-D features)
    Mat predictedLabels25D;
    svm25D->predict(test_data, predictedLabels25D);

    // Predict test set labels (3-D features)
    Mat predictedLabels3D;
    svm3D->predict(reducedTestFeatures, predictedLabels3D);

    // calculate error rate
    float errorNumber = 0;
    std::cout << "25D classify:" << std::endl;
    for(int i = 0; i < predictedLabels25D.rows; i++){
        int c = predictedLabels25D.at<float>(i,0);
        std::cout << c << " ";
        if(c != test_labels[i]){
            errorNumber++;
        }
    }
    std::cout << std::endl;
    std::cout << "error rate :" << errorNumber / 12 << std::endl;

    errorNumber = 0;
    std::cout << "3D classify:" << std::endl;
    for(int i = 0; i < predictedLabels3D.rows; i++){
        int c = predictedLabels3D.at<float>(i,0);
        std::cout << c << " ";
        if(c != test_labels[i]){
            errorNumber++;
        }
    }
    std::cout << std::endl;
    std::cout << "error rate :" << errorNumber / 12 << std::endl;
}



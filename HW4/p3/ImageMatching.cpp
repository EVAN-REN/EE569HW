#include "ImageMatching.h"
#include "RWImage.h"
#include <opencv2/xfeatures2d.hpp>

using namespace cv;
using namespace cv::xfeatures2d;


ImageMatching::ImageMatching(std::string path, int h, int w, int b):height(h), width(w), BytesPerPixel(b){
    // imread PNG image
    Mat mat1 = imread(path + "images/cat_1.png", IMREAD_COLOR);
    Mat mat2 = imread(path + "images/cat_2.png", IMREAD_COLOR);
    Mat mat3 = imread(path + "images/cat_3.png", IMREAD_COLOR);
    Mat mat4 = imread(path + "images/dog_1.png", IMREAD_COLOR);

    if(mat1.empty() || mat2.empty() || mat3.empty() || mat4.empty()) {
        std::cout << "Could not open or find the image." << std::endl;
        exit(0);
    }

    images.push_back(mat1);
    images.push_back(mat2);
    images.push_back(mat3);
    images.push_back(mat4);
}

void ImageMatching::extract_SIFT_Features(int i1, int i2){
    // Initialize the SIFT detector
    Ptr<SIFT> sift = SIFT::create();

    // Detecting keypoints and computing descriptors
    std::vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    sift->detectAndCompute(images[i1], Mat(), keypoints1, descriptors1);
    sift->detectAndCompute(images[i2], Mat(), keypoints2, descriptors2);

    // Matching features
    FlannBasedMatcher matcher;
    std::vector<DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);

    // Filter matches based on Lowe's ratio test
    double ratioThreshold = 0.85;
    std::vector< DMatch > goodMatches;
    for (size_t i = 0; i < matches.size(); i++) {
        if (matches[i].distance < ratioThreshold * matches[i+1].distance) {
            goodMatches.push_back(matches[i]);
        }
    }

    std::vector<KeyPoint> goodKeypoints1, goodKeypoints2;
    Mat goodDescriptors1, goodDescriptors2;
    for (size_t i = 0; i < goodMatches.size(); i++) {
        goodKeypoints1.push_back(keypoints1[goodMatches[i].queryIdx]);
        goodKeypoints2.push_back(keypoints2[goodMatches[i].trainIdx]);
        goodDescriptors1.push_back(descriptors1.row(goodMatches[i].queryIdx));
        goodDescriptors2.push_back(descriptors2.row(goodMatches[i].trainIdx));
        goodMatches[i].queryIdx = i;
        goodMatches[i].trainIdx = i;
    }

    // Plotting matches
    Mat img_matches;
    drawMatches(images[i1], goodKeypoints1, images[i2], goodKeypoints2, goodMatches, img_matches);

    // Display matching results
    imshow("SIFT Matches", img_matches);
    waitKey(0);

    // Find key point with the largest scale
    KeyPoint largestScaleKeypoint;
    int index = 0;
    float largestScale = 0.0;
    for (int i = 0; i < goodKeypoints1.size(); i++) {
        KeyPoint keypoint = goodKeypoints1[i];
        if (keypoint.size > largestScale) {
            largestScale = keypoint.size;
            largestScaleKeypoint = keypoint;
            index = i;
        }
    } 

    // Find closest neighboring key point
    float minDistance = std::numeric_limits<float>::max();
    KeyPoint closestKeypoint;
    for (int i = 0; i < goodKeypoints2.size(); i++) {
        KeyPoint keypoint = goodKeypoints2[i];
        float distance = norm(goodDescriptors1.row(index) - goodDescriptors2.row(i));
        if (distance < minDistance) {
            minDistance = distance;
            closestKeypoint = keypoint;
        }
    }

    // Draw keypoints
    Mat image1WithKeypoints = images[i1];
    Mat image2WithKeypoints;
    // drawKeypoints(images[i1], {largestScaleKeypoint}, image1WithKeypoints);
    Point center(static_cast<int>(largestScaleKeypoint.pt.x), static_cast<int>(largestScaleKeypoint.pt.y));
    circle(image1WithKeypoints, center, static_cast<int>(largestScale), Scalar(0, 0, 255), 2);
    drawKeypoints(images[i2], {closestKeypoint}, image2WithKeypoints);

    imshow("image1 with Largest Scale Keypoint", image1WithKeypoints);
    imshow("image2 with Closest Neighboring Keypoint", image2WithKeypoints);
    waitKey(0);
}

float computeSimilarity(Mat &mat1, Mat &mat2){
    float up = 0;
    float down = 0;
    for(int i = 0; i < mat1.cols; i++){
        up += min(mat1.at<float>(0,i), mat2.at<float>(0,i));
        down += max(mat1.at<float>(0,i), mat2.at<float>(0,i));
    }
    return up / down;
}

void ImageMatching::bagOfWords(){
    // Initialize the SIFT detector
    Ptr<SIFT> sift = SIFT::create();

    // Detecting keypoints and computing descriptors
    std::vector<std::vector<KeyPoint> > keypoints(4, std::vector<KeyPoint>());
    std::vector<Mat> descriptors(4, Mat());
    for(int i = 0; i < 4; i++){
        sift->detectAndCompute(images[i], Mat(), keypoints[i], descriptors[i]);
    }
    
    // PCA
    std::vector<Mat> reducedFeatures(4, Mat());
    for(int i = 0; i < 4; i++){
        PCA pca(descriptors[i], Mat(), PCA::DATA_AS_ROW, 20);
        reducedFeatures[i] = pca.project(descriptors[i]); 
    }
    
    // 构建词汇字典（使用 K-means）
    int k = 8;  // 设置词汇数量
    std::vector<Mat> dictionaries(4, Mat());

    TermCriteria criteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 100, 0.01);
    for(int i = 0; i < 4; i++){
        kmeans(reducedFeatures[i], k, dictionaries[i], criteria, 3, KMEANS_PP_CENTERS);
    }
    
    // create histogram
    std::vector<cv::Mat> histogramFrequencies(4);
    for (int i = 0; i < 4; ++i) {
        histogramFrequencies[i] = cv::Mat::zeros(1, 8, CV_32F);
    }
    for (int i = 0; i < 4; ++i) {
        for(int j = 0; j < dictionaries[i].rows; j++){
            histogramFrequencies[i].at<float>(0,dictionaries[i].at<int>(j,0))++;
        }
        std::cout << "bin" << i << ": " << std::endl;
        for(int j = 0; j < 8; j++){
            histogramFrequencies[i].at<float>(0,j) /= dictionaries[i].rows;
            std::cout << histogramFrequencies[i].at<float>(0,j) << " ";
        }
        std::cout << std::endl;
    }

    // calculate similar
    float similarity1 = computeSimilarity(histogramFrequencies[2], histogramFrequencies[0]);
    float similarity2 = computeSimilarity(histogramFrequencies[2], histogramFrequencies[1]);
    float similarity3 = computeSimilarity(histogramFrequencies[2], histogramFrequencies[3]);

    std::cout << "similarity between cat_3 and cat_1" << std::endl;
    std::cout << similarity1 << std::endl;
    std::cout << "similarity between cat_3 and cat_2" << std::endl;
    std::cout << similarity2 << std::endl;
    std::cout << "similarity between cat_3 and dog_1" << std::endl;
    std::cout << similarity3 << std::endl;
}


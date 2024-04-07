#include "TextureSegmentation.h"
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include "RWImage.h"

using namespace cv;

TextureSegmentation::TextureSegmentation(std::string path, int h, int w, int b):height(h), width(w), BytesPerPixel(b){
    // imread PNG image
    Mat mat = imread(path, IMREAD_GRAYSCALE);

    if(mat.empty()) {
        std::cout << "Could not open or find the image." << std::endl;
        exit(0);
    }

    mat.convertTo(image, CV_32F);
}


double computeEnergy(const Mat& window) {
    double energy = sum(window.mul(window))[0];
    return energy;
}


// calculate energy feature based on window
Mat computeEnergyFeatures(const Mat& image, int windowSize) {
    Mat energyFeatures(image.size(), CV_64F); // stored energy features

    // Mirror boundary extension
    Mat paddedImage;
    copyMakeBorder(image, paddedImage, windowSize / 2, windowSize / 2, windowSize / 2, windowSize / 2, BORDER_REFLECT);

    // Iterate over each pixel in the image
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            // Determine the boundaries of the window
            int top = i;
            int bottom = i + windowSize - 1;
            int left = j;
            int right = j + windowSize - 1;

            // Gets the pixels in the window
            Mat window = paddedImage(Range(top, bottom + 1), Range(left, right + 1));

            // calculate the energy features of the pixels within the window 
            float energy = sqrt(computeEnergy(window) / (windowSize * windowSize));

            // Store the calculated energy eigenvalues to the corresponding locations
            energyFeatures.at<float>(i, j) = energy;
        }
    }

    return energyFeatures;
}

void calculateFilterBankResponses(Mat &image, std::vector<Mat> &filterBankResponses, int windowSize){
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

    // Apply 25 5x5 Laws filters
    for(int j = 0; j < 25; j++){
        Mat filteredImage;
        filter2D(image, filteredImage, -1, lawFilters[j],Point(-1,-1),0,BORDER_REFLECT);
        
        // calculate Energy feature 
        Mat energyFeature = computeEnergyFeatures(image, windowSize);

        filterBankResponses.push_back(energyFeature);
    }
}

void TextureSegmentation::basicMethod(){

    // calculate filterBankResponses
    std::vector<Mat> filterBankResponses;
    calculateFilterBankResponses(image, filterBankResponses, 55);

    Mat features24D;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Mat mat = Mat::ones(1, 24, CV_32F);
            for (int k = 1; k < filterBankResponses.size(); k++) {
                mat.at<float>(0, k - 1) = filterBankResponses[k].at<float>(i,j);
            }
            features24D.push_back(mat);
        }
    }
   
    // texture classification
    int K = 5; // Suppose you have 5 textures
    Mat labels;
    Mat centers;
    TermCriteria criteria(TermCriteria::EPS + TermCriteria::COUNT, 100, 0.1);
    kmeans(features24D, K, labels, criteria, 3, KMEANS_PP_CENTERS, centers);

    Mat result(512, 512, CV_8UC1);
    for(int i = 0; i < 512; i++){
        for(int j = 0; j < 512; j++){
            if(labels.at<int>((i * 512 + j),0) == 0){
                result.at<unsigned char>(i,j) = 0; 
            }else if(labels.at<int>((i * 512 + j),0) == 1){
                result.at<unsigned char>(i,j) = 63; 
            }else if(labels.at<int>((i * 512 + j),0) == 2){
                result.at<unsigned char>(i,j) = 127; 
            }else if(labels.at<int>((i * 512 + j),0) == 3){
                result.at<unsigned char>(i,j) = 191; 
            }else if(labels.at<int>((i * 512 + j),0) == 4){
                result.at<unsigned char>(i,j) = 255; 
            }
        }
    }

    imshow("texture segmentation", result);
    waitKey(0);
    
}

void TextureSegmentation::advancedMethod(){
    // calculate filterBankResponses
    std::vector<Mat> filterBankResponses;
    calculateFilterBankResponses(image, filterBankResponses, 55);

    Mat features24D;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Mat mat = Mat::ones(1, 24, CV_32F);
            for (int k = 1; k < filterBankResponses.size(); k++) {
                mat.at<float>(0, k - 1) = filterBankResponses[k].at<float>(i,j);
            }
            features24D.push_back(mat);
        }
    }

    PCA pca(features24D, Mat(), PCA::DATA_AS_ROW, 3);
    Mat reducedFeatures = pca.project(features24D);      

    int K = 5; // Suppose you have 5 textures
    Mat labels;
    Mat centers;
    TermCriteria criteria(TermCriteria::EPS + TermCriteria::COUNT, 100, 0.1);
    kmeans(reducedFeatures, K, labels, criteria, 10, KMEANS_PP_CENTERS, centers);

    Mat result(512, 512, CV_8UC1);
    std::vector<int> nums(5,0);
    for(int i = 0; i < 512; i++){
        for(int j = 0; j < 512; j++){
            nums[labels.at<int>((i * 512 + j),0)]++;
            if(labels.at<int>((i * 512 + j),0) == 0){
                result.at<unsigned char>(i,j) = 0; 
            }else if(labels.at<int>((i * 512 + j),0) == 1){
                result.at<unsigned char>(i,j) = 63; 
            }else if(labels.at<int>((i * 512 + j),0) == 2){
                result.at<unsigned char>(i,j) = 127; 
            }else if(labels.at<int>((i * 512 + j),0) == 3){
                result.at<unsigned char>(i,j) = 191; 
            }else if(labels.at<int>((i * 512 + j),0) == 4){
                result.at<unsigned char>(i,j) = 255; 
            }
        }
    }
    for(int n : nums){
        std::cout << n << " ";
    }
    

    imshow("texture segmentation", result);
    waitKey(0);
    // Fill small holes
    Mat filledImage;
    int minAreaThreshold = 5;
    morphologyEx(result, filledImage, MORPH_CLOSE, Mat(), Point(-1, -1), 8); // expands and corrodes

    // Remove the small connected regions
    Mat labels1, stats, centroids;
    int numLabels = connectedComponentsWithStats(filledImage, labels1, stats, centroids);
    for (int i = 1; i < numLabels; i++) {
        if (stats.at<int>(i, CC_STAT_AREA) < minAreaThreshold) { // Small connected regions are removed according to the area threshold
            filledImage.setTo(0, labels1 == i);
        }
    }

    // 显示结果
    imshow("Processed Image", filledImage);
    waitKey(0);
}




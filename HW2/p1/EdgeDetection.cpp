#include "EdgeDetection.h"
#include "ColorTrans.h"
#include "RWImage.h"
#include <opencv2/opencv.hpp>

using namespace cv;

EdgeDetection::EdgeDetection(std::string path, int h, int w, int b):height(h), width(w), BytesPerPixel(b){
    RWImage rwi(h, w);
    image = rwi.readRawImage(path, b);
}

void EdgeDetection::sobelED(std::string outputPath, double threshold){

    std::vector<std::vector<std::vector<unsigned char> > > gradient_x(height, std::vector<std::vector<unsigned char> >(width, std::vector<unsigned char>(1, 0)));
    std::vector<std::vector<std::vector<unsigned char> > > gradient_y(height, std::vector<std::vector<unsigned char> >(width, std::vector<unsigned char>(1, 0)));
    std::vector<std::vector<std::vector<unsigned char> > > gradient_m(height, std::vector<std::vector<unsigned char> >(width, std::vector<unsigned char>(1, 0)));

    // convert RGB to GRAY
    ColorTrans ct(height, width);
    std::vector<std::vector<std::vector<unsigned char> > > img = ct.RGB2GRAY(image);

    // calculate gradient magnitudes
    std::vector<std::vector<int> > sobel_x = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    std::vector<std::vector<int> > sobel_y = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};

    int minGradient_x = INT_MAX;
    int maxGradient_x = INT_MIN;
    int minGradient_y = INT_MAX;
    int maxGradient_y = INT_MIN;
    int minGradient_m = INT_MAX;
    int maxGradient_m = INT_MIN;

    for(int i = 1; i < height - 1; i++){
        for(int j = 1; j < width - 1; j++){
            int sum_x = 0, sum_y = 0;
            for(int ki = 0; ki < 3; ki++){
                for(int kj = 0; kj < 3; kj++){
                    sum_x += sobel_x[ki][kj] * img[i - 1 + ki][j - 1 + kj][0];
                    sum_y += sobel_y[ki][kj] * img[i - 1 + ki][j - 1 + kj][0];
                }
            }
            gradient_x[i][j][0] = sum_x;
            gradient_y[i][j][0] = sum_y;
            minGradient_x = std::min(minGradient_x, sum_x);
            maxGradient_x = std::max(maxGradient_x, sum_x);
            minGradient_y = std::min(minGradient_y, sum_y);
            maxGradient_y = std::max(maxGradient_y, sum_y);
            int magnitude = sqrt(sum_x * sum_x + sum_y * sum_y);
            minGradient_m = std::min(minGradient_m, magnitude);
            maxGradient_m = std::max(maxGradient_m, magnitude);
            gradient_m[i][j][0] = magnitude;
        }
    }

    // Normalize
    for(int i = 1; i < height - 1; i++){
        for(int j = 1; j < width - 1; j++){
            gradient_x[i][j][0] = static_cast<unsigned char>((gradient_x[i][j][0] - minGradient_x) * 255 / (maxGradient_x - minGradient_x));
            gradient_y[i][j][0] = static_cast<unsigned char>((gradient_y[i][j][0] - minGradient_y) * 255 / (maxGradient_y - minGradient_y));
            gradient_m[i][j][0] = static_cast<unsigned char>((gradient_m[i][j][0] - minGradient_m) * 255 / (maxGradient_m - minGradient_m));
        }
    }

    // write image
    RWImage rwi(height, width);
    rwi.writeRawImage(outputPath + "_sobel_gx.raw", gradient_x, 1);
    rwi.writeRawImage(outputPath + "_sobel_gy.raw", gradient_y, 1);
    rwi.writeRawImage(outputPath + "_sobel_gm.raw", gradient_m, 1);

    // tune the threshold
    double thresholdValue = 255 * threshold;
    for(int i = 1; i < height - 1; i++){
        for(int j = 1; j < width - 1; j++){
            if(gradient_m[i][j][0] > thresholdValue){
                gradient_m[i][j][0] = 0;
            }else{
                gradient_m[i][j][0] = 255;
            }
        }
    }
    rwi.writeRawImage(outputPath + "_sobel_gmt.raw", gradient_m, 1);


}

void EdgeDetection::cannyED(std::string outputPath, double lowerThreshold, double upperThreshold, int windowSize){

    ColorTrans ct(height, width);
    std::vector<std::vector<std::vector<unsigned char> > > img = ct.RGB2GRAY(image);
    
    cv::Mat matImage(height, width, CV_8UC1);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            matImage.at<unsigned char>(i, j) = img[i][j][0];
        }
    }
    cv::Mat matOutput(height, width, CV_8UC1);

    cv::Canny(matImage, matOutput, lowerThreshold, upperThreshold, windowSize);

    std::vector<std::vector<std::vector<unsigned char> > > resultImage(height, std::vector<std::vector<unsigned char> >(width, std::vector<unsigned char>(1, 0)));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            resultImage[i][j][0] = matOutput.at<unsigned char>(i, j);
        }
    }

    RWImage rwi(height, width);
    rwi.writeRawImage(outputPath + "_canny.raw", resultImage, 1);
    
}

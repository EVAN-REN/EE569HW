#include "NonLocalMeans.h"
#include <opencv2/opencv.hpp>

NonLocalMeans::NonLocalMeans(int h, int w, int b):height(h), width(w), BytesPerPixel(b){

}

// Read image (filename specified by first argument) into image data matrix
void NonLocalMeans::readRawImage(std::string path){
    FILE *file;
	
    if (!(file=fopen(path.c_str(), "rb"))) {
		std::cout << "Cannot open file: " << path << std::endl;
		exit(1);
	}

    unsigned char Imagedata[height][width][BytesPerPixel];
	fread(Imagedata, sizeof(unsigned char), width * height * BytesPerPixel, file);

    image.resize(height, std::vector<std::vector<unsigned char>>(width, std::vector<unsigned char>(BytesPerPixel, 0)));
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            for(int k = 0; k < BytesPerPixel; k++){
                image[i][j][k] = Imagedata[i][j][k];
            }
        }
    }
	fclose(file);
}

// Write image data (filename specified by second argument) from image data matrix
void NonLocalMeans::writeRawImage(std::string path, int type, std::vector<std::vector<std::vector<unsigned char> > > wImage){
    FILE *file;

	if (!(file=fopen(path.c_str(), "wb"))) {
		std::cout << "Cannot open file: " << path << std::endl;
		exit(1);
	}

    // calculate psnr
    std::cout << "NLM_psnr:" << calculatePSNR(wImage, 0) << std::endl;
    
    if(type == 0){
        unsigned char Imagedata[height][width];
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                Imagedata[i][j] = wImage[i][j][0];
            }
        }
     
        fwrite(Imagedata, sizeof(unsigned char), width * height, file);
    }else if(type == 1){
        unsigned char Imagedata[height][width][3];
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                for(int k = 0; k < 3; k++){
                    Imagedata[i][j][k] = wImage[i][j][k];
                }
            }
        }
        fwrite(Imagedata, sizeof(unsigned char), width * height * 3, file);
    }
    
	fclose(file);
}

std::vector<std::vector<std::vector<unsigned char> > > NonLocalMeans::NLM_Function(float intensity, int templateWindowSize, int searchWindowSize){
    std::vector<std::vector<std::vector<unsigned char>>> result(height, std::vector<std::vector<unsigned char>>(width, std::vector<unsigned char>(3, 0)));
    cv::Mat noisyImage(height, width, CV_8U);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            noisyImage.at<unsigned char>(i, j) = image[i][j][0];
        }
    }

    cv::Mat denoisedImage;
    cv::fastNlMeansDenoising(noisyImage, denoisedImage, intensity, templateWindowSize, searchWindowSize);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            result[i][j][0] = denoisedImage.at<unsigned char>(i, j);
        }
    }

    return result;
}

double NonLocalMeans::calculatePSNR(std::vector<std::vector<std::vector<unsigned char> > > compareImage, int channel) {

    double sumSquaredError = 0.0;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int diff = image[i][j][channel] - compareImage[i][j][channel];
            sumSquaredError += diff * diff;
        }
    }

    double mse = sumSquaredError / (width * height);
    double maxPixelValue = 255.0;
    double psnr = 10.0 * log10((maxPixelValue * maxPixelValue) / mse);

    return psnr;
}





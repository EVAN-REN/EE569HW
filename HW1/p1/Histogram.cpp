#include "Histogram.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/plot.hpp>
#include <cmath>

Histogram::Histogram(int h, int w):height(h), width(w), BytesPerPixel(1){

}

// Read image (filename specified by first argument) into image data matrix
void Histogram::readRawImage(std::string path){
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
            image[i][j][0] = Imagedata[i][j][0];
        }
    }
	fclose(file);
}

// Write image data (filename specified by second argument) from image data matrix
void Histogram::writeRawImage(std::string path, int type, std::vector<std::vector<std::vector<unsigned char> > > wImage){
    FILE *file;

	if (!(file=fopen(path.c_str(), "wb"))) {
		std::cout << "Cannot open file: " << path << std::endl;
		exit(1);
	}
    
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

void Histogram::writeRawImage(std::string path){
    FILE *file;

	if (!(file=fopen(path.c_str(), "wb"))) {
		std::cout << "Cannot open file: " << path << std::endl;
		exit(1);
	}
    
    if(BytesPerPixel == 1){
        unsigned char Imagedata[height][width];
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                Imagedata[i][j] = image[i][j][0];
            }
        }
     
        fwrite(Imagedata, sizeof(unsigned char), width * height, file);
    }else if(BytesPerPixel == 3){
        unsigned char Imagedata[height][width][3];
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                for(int k = 0; k < 3; k++){
                    Imagedata[i][j][k] = image[i][j][k];
                }
            }
        }
        fwrite(Imagedata, sizeof(unsigned char), width * height * 3, file);
    }
    
	fclose(file);

}

void Histogram::plotOrignalImage(){
    double pixelArray[256] = {0};
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            pixelArray[image[i][j][0]]++;
        }
    }

    cv::Mat dataMat(1, 256, CV_64F, pixelArray);
    cv::Mat plotResult;
    cv::Ptr<cv::plot::Plot2d> plot = cv::plot::Plot2d::create(dataMat);

    plot->setPlotBackgroundColor(cv::Scalar(255, 255, 255));
    plot->setPlotLineColor(cv::Scalar(0, 0, 255));
    plot->setShowText(true);
    plot->setMinX(0);
    plot->setMaxX(256); 
    plot->setMinY(0);  
    plot->setMaxY(1000);

    plot->render(plotResult);
    cv::copyMakeBorder(plotResult, plotResult, 20, 20, 20, 20, cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));
    cv::namedWindow("Orignal Histogram", cv::WINDOW_AUTOSIZE);
    cv::imshow("Orignal Histogram", plotResult);

    cv::waitKey(0);
}

std::vector<std::vector<std::vector<unsigned char> > > Histogram::transferFunctionBased(){
    std::vector<std::vector<std::vector<unsigned char>>> result(height, std::vector<std::vector<unsigned char>>(width, std::vector<unsigned char>(3, 0)));
    double pixelArray[256] = {0.0};
    // Count the frequency of pixels of each grayscale value
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            pixelArray[image[i][j][0]]++;
        }
    }

    // Calculate the normalized probability histogram
    double totalPixel = width * height;
    for(int i = 0; i < 256; i++){
        pixelArray[i] /= totalPixel;
    }

    // Calculate the CDF
    double accumulate = 0.0;
    for(int i = 0; i < 256; i++){
        accumulate += pixelArray[i];
        pixelArray[i] = accumulate;
    }

    // Create the mapping-table
    int mappingTable[256] = {0};
    for(int i = 0; i < 256; i++){
        mappingTable[i] = static_cast<int>(floor(pixelArray[i] * 255));
        if(mappingTable[i] > 255){
            mappingTable[i] = 255;
        }
    }

    // mapping to image
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            result[i][j][0] = static_cast<unsigned char>(mappingTable[image[i][j][0]]);
        }
    }
    
    return result;
}

std::vector<std::vector<std::vector<unsigned char> > > Histogram::bucketFilling(){
    std::vector<std::vector<std::vector<unsigned char>>> result(height, std::vector<std::vector<unsigned char>>(width, std::vector<unsigned char>(3, 0)));
    int pixelArray[256] = {0};
    // Count the frequency of pixels of each grayscale value
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            pixelArray[image[i][j][0]]++;
        }
    }

    int accumulate = 0;
    int accPixel[256] = {0};
    for(int i = 0; i < 256; i++){
        accPixel[i] = accumulate;
        accumulate += pixelArray[i];
    }

    int binNumber = height * width / 256;
    int pixelNumber[256] = {0};
    
    // mapping to image
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            pixelNumber[image[i][j][0]]++;
            int mapping = (accPixel[image[i][j][0]] + pixelNumber[image[i][j][0]]) / binNumber;
            if(mapping > 255){
                mapping = 255;
            }
            result[i][j][0] = mapping;
        }
    }

    return result;

}
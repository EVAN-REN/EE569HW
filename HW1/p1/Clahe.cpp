#include "Clahe.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

Clahe::Clahe(int h, int w):height(h), width(w), BytesPerPixel(3){

}

// Read image (filename specified by first argument) into image data matrix
void Clahe::readRawImage(std::string path){
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

void Clahe::writeRawImage(std::string path, std::vector<std::vector<std::vector<unsigned char> > > inputImage){
    FILE *file;

	if (!(file=fopen(path.c_str(), "wb"))) {
		std::cout << "Cannot open file: " << path << std::endl;
		exit(1);
	}

    unsigned char Imagedata[height][width][BytesPerPixel];
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            for(int k = 0; k < BytesPerPixel; k++){
                Imagedata[i][j][k] = inputImage[i][j][k];
            }
        }
    }
    fwrite(Imagedata, sizeof(unsigned char), width * height * BytesPerPixel, file);
    
	fclose(file);
}

std::vector<std::vector<std::vector<unsigned char> > > Clahe::RGB2YUV(){
    std::vector<std::vector<std::vector<unsigned char>>> result(height, std::vector<std::vector<unsigned char>>(width, std::vector<unsigned char>(3, 0)));
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            unsigned char y = (0.257 * image[i][j][0]) + (0.504 * image[i][j][1]) + (0.098 * image[i][j][2]) + 16;
            unsigned char u = -(0.148 * image[i][j][0]) - (0.291 * image[i][j][1]) + (0.439 * image[i][j][2]) + 128;
            unsigned char v = (0.439 * image[i][j][0]) - (0.368 * image[i][j][1]) - (0.071 * image[i][j][2]) + 128;
            result[i][j][0] = y;
            result[i][j][1] = u;
            result[i][j][2] = v;
        }
    }
    
    return result;
}

std::vector<std::vector<std::vector<unsigned char> > > Clahe::transferFunctionBased(std::vector<std::vector<std::vector<unsigned char> > > inputImage){
    std::vector<std::vector<std::vector<unsigned char>>> result(height, std::vector<std::vector<unsigned char>>(width, std::vector<unsigned char>(3, 0)));
    double pixelArray[256] = {0.0};
    // Count the frequency of pixels of each grayscale value
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            pixelArray[inputImage[i][j][0]]++;
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
            result[i][j][0] = static_cast<unsigned char>(mappingTable[inputImage[i][j][0]]);
            result[i][j][1] = inputImage[i][j][1];
            result[i][j][2] = inputImage[i][j][2];
        }
    }
    
    return result;
}

std::vector<std::vector<std::vector<unsigned char> > > Clahe::bucketFilling(std::vector<std::vector<std::vector<unsigned char> > > inputImage){
    std::vector<std::vector<std::vector<unsigned char>>> result(height, std::vector<std::vector<unsigned char>>(width, std::vector<unsigned char>(3, 0)));
    int pixelArray[256] = {0};
    // Count the frequency of pixels of each grayscale value
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            pixelArray[inputImage[i][j][0]]++;
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
            pixelNumber[inputImage[i][j][0]]++;
            int mapping = (accPixel[inputImage[i][j][0]] + pixelNumber[inputImage[i][j][0]]) / binNumber;
            if(mapping > 255){
                mapping = 255;
            }
            result[i][j][0] = mapping;
            result[i][j][1] = inputImage[i][j][1];
            result[i][j][2] = inputImage[i][j][2];
        }
    }

    return result;
}

std::vector<std::vector<std::vector<unsigned char> > > Clahe::YUV2RGB(std::vector<std::vector<std::vector<unsigned char> > > inputImage){
    std::vector<std::vector<std::vector<unsigned char>>> result(height, std::vector<std::vector<unsigned char>>(width, std::vector<unsigned char>(3, 0)));
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            unsigned char r = std::max(0, std::min(255, static_cast<int>(1.164 * (inputImage[i][j][0] - 16) + 1.596 * (inputImage[i][j][2] - 128))));
            unsigned char g = std::max(0, std::min(255, static_cast<int>(1.164 * (inputImage[i][j][0] - 16) - 0.813 * (inputImage[i][j][2] - 128) - 0.391 * (inputImage[i][j][1] - 128))));
            unsigned char b = std::max(0, std::min(255, static_cast<int>(1.164 * (inputImage[i][j][0] - 16) + 2.018 * (inputImage[i][j][1] - 128))));
            result[i][j][0] = r;
            result[i][j][1] = g;
            result[i][j][2] = b;
        }
    }
    return result;
}

std::vector<std::vector<std::vector<unsigned char> > > Clahe::cvCLAHE(std::vector<std::vector<std::vector<unsigned char> > > inputImage){
    std::vector<std::vector<std::vector<unsigned char>>> result(height, std::vector<std::vector<unsigned char>>(width, std::vector<unsigned char>(3, 0)));
    cv::Mat mat(height, width, CV_8U);

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            mat.at<unsigned char>(i, j) = inputImage[i][j][0];
        }
    }

    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(2,cv::Size(12,12));
    clahe->apply(mat, mat);

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            result[i][j][0] = mat.at<unsigned char>(i, j);
            result[i][j][1] = inputImage[i][j][1];
            result[i][j][2] = inputImage[i][j][2];
        }
    }
    return result;

}
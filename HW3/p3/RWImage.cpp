#include "RWImage.h"

RWImage::RWImage(int h, int w):height(h), width(w){

}

// Read image (filename specified by first argument) into image data matrix
std::vector<std::vector<std::vector<unsigned char> > > RWImage::readRawImage(std::string path, int BytesPerPixel){
    FILE *file;
	
    if (!(file=fopen(path.c_str(), "rb"))) {
		std::cout << "Cannot open file: " << path << std::endl;
		exit(1);
	}

    unsigned char Imagedata[height][width][BytesPerPixel];
	fread(Imagedata, sizeof(unsigned char), width * height * BytesPerPixel, file);

    std::vector<std::vector<std::vector<unsigned char> > > result(height, std::vector<std::vector<unsigned char> >(width, std::vector<unsigned char>(BytesPerPixel, 0)));

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            for(int k = 0; k < BytesPerPixel; k++){
                result[i][j][k] = Imagedata[i][j][k];
            }
        }
    }
	fclose(file);

    return result;
}

void RWImage::writeRawImage(std::string path, std::vector<std::vector<std::vector<unsigned char> > > inputImage, int BytesPerPixel){
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

cv::Mat RWImage::transformToMat(std::vector<std::vector<std::vector<unsigned char> > > image){
    Mat result(height, width, CV_8UC1);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            result.at<unsigned char>(i, j) = image[i][j][0];
        }
    }

    return result;
}
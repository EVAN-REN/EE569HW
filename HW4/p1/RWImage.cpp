#include "RWImage.h"

RWImage::RWImage(int h, int w):height(h), width(w){

}

// Read image (filename specified by first argument) into image data matrix
Mat RWImage::readRawImage(std::string path, int BytesPerPixel){
    FILE *file;
	
    if (!(file=fopen(path.c_str(), "rb"))) {
		std::cout << "Cannot open file: " << path << std::endl;
		exit(1);
	}

    unsigned char Imagedata[height][width][BytesPerPixel];
	fread(Imagedata, sizeof(unsigned char), width * height * BytesPerPixel, file);

    Mat result(height, width, CV_32F);

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            result.at<float>(i,j) = Imagedata[i][j][0];
        }
    }
	fclose(file);

    return result;
}

void RWImage::writeRawImage(std::string path, Mat inputImage, int BytesPerPixel){
    FILE *file;

	if (!(file=fopen(path.c_str(), "wb"))) {
		std::cout << "Cannot open file: " << path << std::endl;
		exit(1);
	}

    unsigned char Imagedata[height][width][BytesPerPixel];
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            Imagedata[i][j][0] = inputImage.at<float>(i,j);
        }
    }
    fwrite(Imagedata, sizeof(unsigned char), width * height * BytesPerPixel, file);
    
	fclose(file);
}



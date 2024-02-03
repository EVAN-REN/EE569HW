#include "Basic.h"
#include <cmath>

Basic::Basic(int h, int w, int b):height(h), width(w), BytesPerPixel(b){

}

// Read image (filename specified by first argument) into image data matrix
void Basic::readRawImage(std::string path){
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
void Basic::writeRawImage(std::string path, int type, std::vector<std::vector<std::vector<unsigned char> > > wImage){
    FILE *file;

	if (!(file=fopen(path.c_str(), "wb"))) {
		std::cout << "Cannot open file: " << path << std::endl;
		exit(1);
	}

    // calculate psnr
    std::cout << "Basic_psnr:" << calculatePSNR(wImage, 0) << std::endl;
    
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

std::vector<std::vector<std::vector<unsigned char> > > Basic::uniformWeight(){
    std::vector<std::vector<std::vector<unsigned char>>> result(height, std::vector<std::vector<unsigned char>>(width, std::vector<unsigned char>(3, 0)));
    
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(i == 0 || i == height - 1 || j == 0 || j == width - 1){
                result[i][j] = image[i][j];
            }else{
                result[i][j][0] = (image[i - 1][j - 1][0] + image[i][j - 1][0] + image[i + 1][j - 1][0] + image[i - 1][j][0] + image[i][j][0] + image[i + 1][j][0] + image[i - 1][j + 1][0] + image[i][j + 1][0] + image[i + 1][j + 1][0]) / 9; 
            }
            
        }
    }
    
    return result;
}

double gaussianWeight(int x, int y, double sigma) {
    return exp(-(x * x + y * y) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma);
}


std::vector<std::vector<std::vector<unsigned char> > >  Basic::GaussianWeight(int lenSide, double sigma){
    std::vector<std::vector<std::vector<unsigned char>>> result(height, std::vector<std::vector<unsigned char>>(width, std::vector<unsigned char>(3, 0)));
    std::vector<std::vector<double>> gs(lenSide, std::vector<double>(lenSide, 0));
    for(int i = 0; i < lenSide; i++){
        for(int j = 0; j <= i; j++){
            gs[i][j] = gaussianWeight(abs((lenSide / 2) - i), abs((lenSide / 2) - j), sigma);
            gs[j][i] = gs[i][j];
        }
    }
    double totolWeight = 0.0;
    for(int i = 0; i < lenSide; i++){
        for(int j = 0; j < lenSide; j++){
            totolWeight += gs[i][j];
        }
    }
    for(int i = 0; i < lenSide; i++){
        for(int j = 0; j < lenSide; j++){
            gs[i][j] = gs[i][j] / totolWeight;
        }
    }

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if((i - (lenSide / 2)) < 0 || (i + (lenSide / 2)) > (height - 1) || (j - (lenSide / 2)) < 0 || (j + (lenSide / 2)) > (width - 1)){
                result[i][j] = image[i][j];
            }else{
                for(int k1 = 0; k1 < lenSide; k1++){
                    for(int k2 = 0; k2 < lenSide; k2++){
                        result[i][j][0] += (gs[k1][k2] * image[i + k1 - (lenSide / 2)][j + k2 - (lenSide / 2)][0]); 
                    }
                }
            }
            
        }
    }
    
    return result;
}


double Basic::calculatePSNR(std::vector<std::vector<std::vector<unsigned char> > > compareImage, int channel) {

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



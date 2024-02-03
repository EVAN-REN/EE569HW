#include "WaterColor.h"
#include <cmath>

WaterColor::WaterColor(int h, int w, int b):height(h), width(w), BytesPerPixel(b){

}

// Read image (filename specified by first argument) into image data matrix
void WaterColor::readRawImage(std::string path){
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
void WaterColor::writeRawImage(std::string path, int type, std::vector<std::vector<std::vector<unsigned char> > > wImage){
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


double gWeight(int x, int y, double sigma) {
    return exp(-(x * x + y * y) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma);
}


std::vector<std::vector<std::vector<unsigned char> > >  WaterColor::GaussianFilter(std::vector<std::vector<std::vector<unsigned char> > > inputImage, int lenSide, double sigma){
    std::vector<std::vector<std::vector<unsigned char>>> result(height, std::vector<std::vector<unsigned char>>(width, std::vector<unsigned char>(3, 0)));
    std::vector<std::vector<double>> gs(lenSide, std::vector<double>(lenSide, 0));
    for(int i = 0; i < lenSide; i++){
        for(int j = 0; j <= i; j++){
            gs[i][j] = gWeight(abs((lenSide / 2) - i), abs((lenSide / 2) - j), sigma);
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
                result[i][j] = inputImage[i][j];
            }else{
                for(int k1 = 0; k1 < lenSide; k1++){
                    for(int k2 = 0; k2 < lenSide; k2++){
                        result[i][j][0] += (gs[k1][k2] * inputImage[i + k1 - (lenSide / 2)][j + k2 - (lenSide / 2)][0]); 
                        result[i][j][1] = inputImage[i][j][1];
                        result[i][j][2] = inputImage[i][j][2];
                    }
                }
            }
            
        }
    }
    
    return result;
}

double bilateralWeight(std::vector<std::vector<std::vector<unsigned char> > > &inputImage, int x, int y, int x0, int y0, double sigmac, double sigmas) {
    return exp(-(((x - x0) * (x - x0) + (y - y0) * (y - y0)) / (2 * sigmac * sigmac)) - ((inputImage[x][y][0] - inputImage[x0][y0][0]) * (inputImage[x][y][0] - inputImage[x0][y0][0]) / (2 * sigmas * sigmas)));
}


std::vector<std::vector<std::vector<unsigned char> > >  WaterColor::bilateralFilter(std::vector<std::vector<std::vector<unsigned char> > > inputImage, int lenSide, double sigmac, double sigmas){
    std::vector<std::vector<std::vector<unsigned char>>> result(height, std::vector<std::vector<unsigned char>>(width, std::vector<unsigned char>(3, 0)));

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if((i - (lenSide / 2)) < 0 || (i + (lenSide / 2)) > (height - 1) || (j - (lenSide / 2)) < 0 || (j + (lenSide / 2)) > (width - 1)){
                result[i][j] = inputImage[i][j];
            }else{
                std::vector<std::vector<double>> bi(lenSide, std::vector<double>(lenSide, 0));
                for(int k1 = 0; k1 < lenSide; k1++){
                    for(int k2 = 0; k2 < lenSide; k2++){
                        bi[k1][k2] = bilateralWeight(inputImage, i - (lenSide / 2) + k1, j - (lenSide / 2) + k2, i, j, sigmac, sigmas);
                    }
                }
                double totolWeight = 0.0;
                for(int k1 = 0; k1 < lenSide; k1++){
                    for(int k2 = 0; k2 < lenSide; k2++){
                        totolWeight += bi[k1][k2];
                    }
                }
                for(int k1 = 0; k1 < lenSide; k1++){
                    for(int k2 = 0; k2 < lenSide; k2++){
                        result[i][j][0] += (bi[k1][k2] * inputImage[i + k1 - (lenSide / 2)][j + k2 - (lenSide / 2)][0] / totolWeight); 
                    }
                }
                result[i][j][1] = inputImage[i][j][1];
                result[i][j][2] = inputImage[i][j][2];
            }
            
        }
    }
    
    return result;
}

std::vector<std::vector<std::vector<unsigned char> > > WaterColor::RGB2YUV(){
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

std::vector<std::vector<std::vector<unsigned char> > > WaterColor::YUV2RGB(std::vector<std::vector<std::vector<unsigned char> > > inputImage){
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

std::vector<std::vector<std::vector<unsigned char> > >  WaterColor::medianFilter(std::vector<std::vector<std::vector<unsigned char> > > inputImage, int windowSize){
    std::vector<std::vector<std::vector<unsigned char>>> result(height, std::vector<std::vector<unsigned char>>(width, std::vector<unsigned char>(3, 0)));
    
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if((i - (windowSize / 2)) < 0 || (i + (windowSize / 2)) > (height - 1) || (j - (windowSize / 2)) < 0 || (j + (windowSize / 2)) > (width - 1)){
                result[i][j] = inputImage[i][j];
            }else{
                std::vector<unsigned char> v;

                for(int k1 = 0; k1 < windowSize; k1++){
                    for(int k2 = 0; k2 < windowSize; k2++){
                        v.push_back(inputImage[i - (windowSize / 2) + k1][j - (windowSize / 2) + k2][0]);
                    }
                }

                std::nth_element(v.begin(), v.begin() + v.size() / 2, v.end());
                result[i][j][0] = v[v.size() / 2];
                result[i][j][1] = inputImage[i][j][1];
                result[i][j][2] = inputImage[i][j][2];
                
            }           
        }
    }
    
    return result;
}

double WaterColor::calculatePSNR(std::vector<std::vector<std::vector<unsigned char> > > compareImage, int channel) {

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

std::vector<std::vector<std::vector<unsigned char> > >  WaterColor::Linear_Combination(std::vector<std::vector<std::vector<unsigned char> > > inputImage1, double x1, std::vector<std::vector<std::vector<unsigned char> > > inputImage2, double x2){
    std::vector<std::vector<std::vector<unsigned char>>> result(height, std::vector<std::vector<unsigned char>>(width, std::vector<unsigned char>(3, 0)));
    
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            
            result[i][j][0] = std::min(static_cast<int>(std::floor(x1 * inputImage1[i][j][0] + x2 * inputImage2[i][j][0])), 255);
            result[i][j][1] = inputImage1[i][j][1];
            result[i][j][2] = inputImage1[i][j][2];
        }
    }

    return result;
}
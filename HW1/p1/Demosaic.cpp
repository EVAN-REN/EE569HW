#include "Demosaic.h"

Demosaic::Demosaic(int h, int w):height(h), width(w), BytesPerPixel(1){

}

// Read image (filename specified by first argument) into image data matrix
void Demosaic::readRawImage(std::string path){
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
void Demosaic::writeRawImage(std::string path){
    FILE *file;

	if (!(file=fopen(path.c_str(), "wb"))) {
		std::cout << "Cannot open file: " << path << std::endl;
		exit(1);
	}
    unsigned char Imagedata[height][width][3];
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            for(int k = 0; k < 3; k++){
                Imagedata[i][j][k] = image[i][j][k];
            }
        }
    }
	fwrite(Imagedata, sizeof(unsigned char), width * height * 3, file);
	fclose(file);
}

// zero padding
std::vector<std::vector<std::vector<unsigned char>>> Demosaic::zeroPadding(){
    std::vector<std::vector<std::vector<unsigned char>>> result(height + 2, std::vector<std::vector<unsigned char>>(width + 2, std::vector<unsigned char>(BytesPerPixel, 0)));
    for(int i = 0; i < height + 2; i++){
        for(int j = 0; j < width + 2; j++){
            if(i == 0 || i == height + 1 || j == 0 || j == width + 1){
                result[i][j][0] = 0;
            }else{
                result[i][j][0] = image[i - 1][j - 1][0];
            }
        }
    }
    return result;
}

// mirror reflection(odd)
std::vector<std::vector<std::vector<unsigned char>>> Demosaic::mirrorReflection(){
    std::vector<std::vector<std::vector<unsigned char>>> result(height + 2, std::vector<std::vector<unsigned char>>(width + 2, std::vector<unsigned char>(BytesPerPixel, 0)));
    int num = 0;
    for(int i = 0; i < height + 2; i++){
        for(int j = 0; j < width + 2; j++){
            if(i == 0 && j == 0){
                result[i][j][0] = image[1][1][0];
            }else if(i == 0 && j == width + 1){
                result[i][j][0] = image[1][width - 2][0];
            }else if(i == height + 1 && j == 0){
                result[i][j][0] = image[height - 2][1][0];
            }else if(i == height + 1 && j == width + 1){
                result[i][j][0] = image[height - 2][width - 2][0]; 
            }else if(i == 0){
                result[i][j][0] = image[1][j - 1][0];
            }else if(i == height + 1){
                result[i][j][0] = image[height - 2][j - 1][0];
            }else if(j == 0){
                result[i][j][0] = image[i - 1][1][0];
            }else if(j == width + 1){
                result[i][j][0] = image[i - 1][width - 2][0];
            }else{
                result[i][j][0] = image[i - 1][j - 1][0];
            }
        }
    }
    return result;
}

void Demosaic::Bilinear_Demosaicing(std::vector<std::vector<std::vector<unsigned char>>> oriImage){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            // green
            if(i % 2 == 0 && j % 2 == 0){
                image[i][j][0] = (oriImage[i + 1][j][0] + oriImage[i + 1][j + 2][0]) / 2;
                image[i][j][1] = oriImage[i + 1][j + 1][0];
                image[i][j][2] = (oriImage[i][j + 1][0] + oriImage[i + 2][j + 1][0]) / 2;
            // green
            }else if(i % 2 == 1 && j % 2 == 1){
                image[i][j][0] = (oriImage[i][j + 1][0] + oriImage[i + 2][j + 1][0]) / 2;
                image[i][j][1] = oriImage[i + 1][j + 1][0];
                image[i][j][2] = (oriImage[i + 1][j][0] + oriImage[i + 1][j + 2][0]) / 2;
            // red
            }else if(i % 2 == 0 && j % 2 == 1){
                image[i][j][0] = oriImage[i + 1][j + 1][0];
                image[i][j][1] = (oriImage[i][j + 1][0] + oriImage[i + 1][j][0] + oriImage[i + 1][j + 2][0] + oriImage[i + 2][j + 1][0]) / 4;
                image[i][j][2] = (oriImage[i][j][0] + oriImage[i + 2][j + 2][0] + oriImage[i][j + 2][0] + oriImage[i + 2][j][0]) / 4;
            // blue
            }else{
                image[i][j][0] = (oriImage[i][j][0] + oriImage[i + 2][j + 2][0] + oriImage[i][j + 2][0] + oriImage[i + 2][j][0]) / 4;
                image[i][j][1] = (oriImage[i][j + 1][0] + oriImage[i + 1][j][0] + oriImage[i + 1][j + 2][0] + oriImage[i + 2][j + 1][0]) / 4;
                image[i][j][2] = oriImage[i + 1][j + 1][0];
            }
        }
    }
}
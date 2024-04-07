#include "HalfToned.h"
#include "RWImage.h"
#include <random>
#include <cmath>
#include <algorithm>


HalfToned::HalfToned(std::string path, int h, int w, int b):height(h), width(w), BytesPerPixel(b){
    RWImage rwi(h, w);
    image = rwi.readRawImage(path, b);
}

void HalfToned::fixedThresholding(std::string outputPath, double threshold){
    std::vector<std::vector<std::vector<unsigned char> > > result(height, std::vector<std::vector<unsigned char> >(width, std::vector<unsigned char>(1, 0)));

    // fixed thresholding
    int thresholdValue = threshold * 255;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(image[i][j][0] > thresholdValue){
                result[i][j][0] = 255;
            }else{
                result[i][j][0] = 0;
            }
        }
    }

    // write image
    RWImage rwi(height, width);
    rwi.writeRawImage(outputPath, result, 1);

}

void HalfToned::randomThresholding(std::string outputPath){
    std::vector<std::vector<std::vector<unsigned char> > > result(height, std::vector<std::vector<unsigned char> >(width, std::vector<unsigned char>(1, 0)));

    // random thresholding
    std::random_device rd;
    // Mersenne Twister to create random number
    std::mt19937 gen(rd());

    int minThreshold = 0;
    int maxThreshold = 255;
    std::uniform_int_distribution<> dis(minThreshold, maxThreshold);

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            int thresholdValue = dis(gen);
            if(image[i][j][0] < thresholdValue){
                result[i][j][0] = 0;
            }else{
                result[i][j][0] = 255;
            }
        }
    }

    // write image
    RWImage rwi(height, width);
    rwi.writeRawImage(outputPath, result, 1);

}

std::vector<std::vector<int> > expendMatrix(std::vector<std::vector<int> > input){
    int inputSize = input.size();
    int outputSize = inputSize * 2;
    std::vector<std::vector<int> > output(outputSize, std::vector<int>(outputSize, 0));

    for(int i = 0; i < outputSize; i++){
        for(int j = 0; j < outputSize; j++){
            int x = i / inputSize;
            int y = j / inputSize;
            if(x == 0 && y == 0){
                output[i][j] = 4 * input[i][j] + 1;
            }else if(x == 1 && y == 0){
                output[i][j] = 4 * input[i % inputSize][j] + 3;
            }else if(x == 0 && y == 1){
                output[i][j] = 4 * input[i][j % inputSize] + 2;
            }else{
                output[i][j] = 4 * input[i % inputSize][j % inputSize];
            }
        }
    }

    return output;
}

void HalfToned::ditheringMatrix(std::string outputPath, int windowSize){
    std::vector<std::vector<std::vector<unsigned char> > > result(height, std::vector<std::vector<unsigned char> >(width, std::vector<unsigned char>(1, 0)));

    int n = log2(windowSize);
    // calculate matirx
    std::vector<std::vector<int> > matrix(2, std::vector<int>(2, 0));
    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[1][0] = 3;
    matrix[1][1] = 0;
    
    for(int i = 1; i < n; i++){
        matrix = expendMatrix(matrix);
    }

    // convert to threshold matrix
    for(int i = 0; i < windowSize; i++){
        for(int j = 0; j < windowSize; j++){
            matrix[i][j] = (matrix[i][j] + 0.5) * 255 / (windowSize * windowSize);
        }
    }

    // compare to threshold
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(image[i][j][0] < matrix[i % windowSize][j % windowSize]){
                result[i][j][0] = 0;
            }else{
                result[i][j][0] = 255;
            }
        }
    }

    // write image
    RWImage rwi(height, width);
    rwi.writeRawImage(outputPath, result, 1);

}

void HalfToned::errorDiffusion_FS(std::string outputPath){
    std::vector<std::vector<std::vector<unsigned char> > > result = image;

    for(int i = 0; i < height; i++){
        if(i % 2 == 0){
            for(int j = 0; j < width; j++){
                int oldPixel = result[i][j][0];
                int newPixel = (oldPixel > 127) ? 255 : 0;
                result[i][j][0] = newPixel;
                int error = oldPixel - newPixel;

                if (j < width - 1){
                    result[i][j + 1][0] += error * 7 / 16;
                }
                if (j < width - 1 && i < height - 1){
                    result[i + 1][j + 1][0] += error / 16;
                }
                if(i < height - 1){
                    result[i + 1][j][0] += error * 5 / 16;
                }
                if(i < height - 1 && j > 0){
                    result[i + 1][j - 1][0] += error * 3 / 16;
                }       
            }
        }else{
            for(int j = width - 1; j >= 0; j--){
                int oldPixel = result[i][j][0];
                int newPixel = (oldPixel > 127) ? 255 : 0;
                result[i][j][0] = newPixel;
                int error = oldPixel - newPixel;

                if (j < width - 1 && i < height - 1){
                    result[i + 1][j + 1][0] += error * 3 / 16;
                }
                if(i < height - 1){
                    result[i + 1][j][0] += error * 5 / 16;
                }
                if(i < height - 1 && j > 0){
                    result[i + 1][j - 1][0] += error / 16;
                } 
                if(j > 0){
                    result[i][j - 1][0] += error * 7 / 16;
                }     
            }
        }
    }

    // write image
    RWImage rwi(height, width);
    rwi.writeRawImage(outputPath, result, 1);
}

void HalfToned::errorDiffusion_JJN(std::string outputPath){
    std::vector<std::vector<std::vector<unsigned char> > > result = image;

    for(int i = 0; i < height; i++){
        if(i % 2 == 0){
            for(int j = 0; j < width; j++){
                int oldPixel = result[i][j][0];
                int newPixel = (oldPixel > 127) ? 255 : 0;
                result[i][j][0] = newPixel;
                int error = oldPixel - newPixel;

                if (j < width - 1){
                    result[i][j + 1][0] += error * 7 / 48;
                }
                if (j < width - 2){
                    result[i][j + 2][0] += error * 5 / 48;
                }
                if(i < height - 1){
                    result[i + 1][j][0] += error * 7 / 48;
                }
                if(i < height - 2){
                    result[i + 2][j][0] += error * 5 / 48;
                }
                if(i < height - 1 && j < width - 1){
                    result[i + 1][j + 1][0] += error * 5 / 48;
                }
                if(i < height - 1 && j < width - 2){
                    result[i + 1][j + 2][0] += error * 3 / 48;
                }
                if(i < height - 2 && j < width - 1){
                    result[i + 2][j + 1][0] += error * 3 / 48;
                }
                if(i < height - 2 && j < width - 2){
                    result[i + 2][j + 2][0] += error / 48;
                }
                if(i < height - 1 && j > 0){
                    result[i + 1][j - 1][0] += error * 5 / 48;
                }
                if(i < height - 1 && j > 1){
                    result[i + 1][j - 2][0] += error * 3 / 48;
                }
                if(i < height - 2 && j > 0){
                    result[i + 2][j - 1][0] += error * 3 / 48;
                }
                if(i < height - 2 && j > 1){
                    result[i + 2][j - 2][0] += error / 48;
                }
            }
        }else{
            for(int j = width - 1; j >= 0; j--){
                int oldPixel = result[i][j][0];
                int newPixel = (oldPixel > 127) ? 255 : 0;
                result[i][j][0] = newPixel;
                int error = oldPixel - newPixel;

                if (j > 0){
                    result[i][j - 1][0] += error * 7 / 48;
                }
                if (j > 1){
                    result[i][j - 2][0] += error * 5 / 48;
                }
                if(i < height - 1){
                    result[i + 1][j][0] += error * 7 / 48;
                }
                if(i < height - 2){
                    result[i + 2][j][0] += error * 5 / 48;
                }
                if(i < height - 1 && j < width - 1){
                    result[i + 1][j + 1][0] += error * 5 / 48;
                }
                if(i < height - 1 && j < width - 2){
                    result[i + 1][j + 2][0] += error * 3 / 48;
                }
                if(i < height - 2 && j < width - 1){
                    result[i + 2][j + 1][0] += error * 3 / 48;
                }
                if(i < height - 2 && j < width - 2){
                    result[i + 2][j + 2][0] += error / 48;
                }
                if(i < height - 1 && j > 0){
                    result[i + 1][j - 1][0] += error * 5 / 48;
                }
                if(i < height - 1 && j > 1){
                    result[i + 1][j - 2][0] += error * 3 / 48;
                }
                if(i < height - 2 && j > 0){
                    result[i + 2][j - 1][0] += error * 3 / 48;
                }
                if(i < height - 2 && j > 1){
                    result[i + 2][j - 2][0] += error / 48;
                }   
            }
        }
    }

    // write image
    RWImage rwi(height, width);
    rwi.writeRawImage(outputPath, result, 1);
}

void HalfToned::errorDiffusion_Stu(std::string outputPath){
    std::vector<std::vector<std::vector<unsigned char> > > result = image;

    for(int i = 0; i < height; i++){
        if(i % 2 == 0){
            for(int j = 0; j < width; j++){
                int oldPixel = result[i][j][0];
                int newPixel = (oldPixel > 127) ? 255 : 0;
                result[i][j][0] = newPixel;
                int error = oldPixel - newPixel;

                if (j < width - 1){
                    result[i][j + 1][0] += error * 8 / 42;
                }
                if (j < width - 2){
                    result[i][j + 2][0] += error * 4 / 42;
                }
                if(i < height - 1){
                    result[i + 1][j][0] += error * 8 / 42;
                }
                if(i < height - 2){
                    result[i + 2][j][0] += error * 4 / 42;
                }
                if(i < height - 1 && j < width - 1){
                    result[i + 1][j + 1][0] += error * 4 / 42;
                }
                if(i < height - 1 && j < width - 2){
                    result[i + 1][j + 2][0] += error * 2 / 42;
                }
                if(i < height - 2 && j < width - 1){
                    result[i + 2][j + 1][0] += error * 2 / 42;
                }
                if(i < height - 2 && j < width - 2){
                    result[i + 2][j + 2][0] += error / 42;
                }
                if(i < height - 1 && j > 0){
                    result[i + 1][j - 1][0] += error * 4 / 42;
                }
                if(i < height - 1 && j > 1){
                    result[i + 1][j - 2][0] += error * 2 / 42;
                }
                if(i < height - 2 && j > 0){
                    result[i + 2][j - 1][0] += error * 2 / 42;
                }
                if(i < height - 2 && j > 1){
                    result[i + 2][j - 2][0] += error / 42;
                }
            }
        }else{
            for(int j = width - 1; j >= 0; j--){
                int oldPixel = result[i][j][0];
                int newPixel = (oldPixel > 127) ? 255 : 0;
                result[i][j][0] = newPixel;
                int error = oldPixel - newPixel;

                if (j > 0){
                    result[i][j - 1][0] += error * 8 / 42;
                }
                if (j > 1){
                    result[i][j - 2][0] += error * 4 / 42;
                }
                if(i < height - 1){
                    result[i + 1][j][0] += error * 8 / 42;
                }
                if(i < height - 2){
                    result[i + 2][j][0] += error * 4 / 42;
                }
                if(i < height - 1 && j < width - 1){
                    result[i + 1][j + 1][0] += error * 4 / 42;
                }
                if(i < height - 1 && j < width - 2){
                    result[i + 1][j + 2][0] += error * 2 / 42;
                }
                if(i < height - 2 && j < width - 1){
                    result[i + 2][j + 1][0] += error * 2 / 42;
                }
                if(i < height - 2 && j < width - 2){
                    result[i + 2][j + 2][0] += error / 42;
                }
                if(i < height - 1 && j > 0){
                    result[i + 1][j - 1][0] += error * 4 / 42;
                }
                if(i < height - 1 && j > 1){
                    result[i + 1][j - 2][0] += error * 2 / 42;
                }
                if(i < height - 2 && j > 0){
                    result[i + 2][j - 1][0] += error * 2 / 42;
                }
                if(i < height - 2 && j > 1){
                    result[i + 2][j - 2][0] += error / 42;
                }   
            }
        }
    }

    // write image
    RWImage rwi(height, width);
    rwi.writeRawImage(outputPath, result, 1);
}
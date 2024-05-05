#include "ErrorDiffusion.h"
#include "RWImage.h"
#include <random>
#include <cmath>
#include <algorithm>


ErrorDiffusion::ErrorDiffusion(std::string path, int h, int w, int b):height(h), width(w), BytesPerPixel(b){
    RWImage rwi(h, w);
    image = rwi.readRawImage(path, b);
}

void ErrorDiffusion::separableED(std::string outputPath){
    // rgb2cmy
    std::vector<std::vector<std::vector<unsigned char> > > cmy(height, std::vector<std::vector<unsigned char> >(width, std::vector<unsigned char>(3, 0)));
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            for(int k = 0; k < 3; k++){
                cmy[i][j][k] = 255 - image[i][j][k];
            }
        }
    }

    // Floyd-Steinberg error diffusion
    for(int k = 0; k < 3; k++){
        for(int i = 0; i < height; i++){
            if(i % 2 == 0){
                for(int j = 0; j < width; j++){
                    int oldPixel = cmy[i][j][k];
                    int newPixel = (oldPixel > 127) ? 255 : 0;
                    cmy[i][j][k] = newPixel;
                    int error = oldPixel - newPixel;

                    if (j < width - 1){
                        cmy[i][j + 1][k] += error * 7 / 16;
                    }
                    if (j < width - 1 && i < height - 1){
                        cmy[i + 1][j + 1][k] += error / 16;
                    }
                    if(i < height - 1){
                        cmy[i + 1][j][k] += error * 5 / 16;
                    }
                    if(i < height - 1 && j > 0){
                        cmy[i + 1][j - 1][k] += error * 3 / 16;
                    }       
                }
            }else{
                for(int j = width - 1; j >= 0; j--){
                    int oldPixel = cmy[i][j][k];
                    int newPixel = (oldPixel > 127) ? 255 : 0;
                    cmy[i][j][k] = newPixel;
                    int error = oldPixel - newPixel;

                    if (j < width - 1 && i < height - 1){
                        cmy[i + 1][j + 1][k] += error * 3 / 16;
                    }
                    if(i < height - 1){
                        cmy[i + 1][j][k] += error * 5 / 16;
                    }
                    if(i < height - 1 && j > 0){
                        cmy[i + 1][j - 1][k] += error / 16;
                    } 
                    if(j > 0){
                        cmy[i][j - 1][k] += error * 7 / 16;
                    }     
                }
            }
        }
    }

    // cmy2rgb
    std::vector<std::vector<std::vector<unsigned char> > > result(height, std::vector<std::vector<unsigned char> >(width, std::vector<unsigned char>(3, 0)));
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            for(int k = 0; k < 3; k++){
                result[i][j][k] = 255 - cmy[i][j][k];
            }
        }
    }
    
    // write image
    RWImage rwi(height, width);
    rwi.writeRawImage(outputPath, result, 3);
}

int distanceSquared(int r1, int g1, int b1, int r2, int g2, int b2) {
    int dr = r1 - r2;
    int dg = g1 - g2;
    int db = b1 - b2;
    return dr * dr + dg * dg + db * db;
}

// CMYW:1 MYGC:2 RGMY:3 KRGB:4 RGBM:5 CMGB:6
int locateMBVQ(int r, int g, int b){
    if((r + g) > 255){
        if((g + b > 255)){
            if((r + g + b) > 510){
                return 1;
            }else{
                return 2;
            }
        }else{
            return 3;
        }
    }else{
        if(!((g + b) > 255)){
            if(!((r + g + b) > 255)){
                return 4;
            }else{
                return 5;
            }
        }else{
            return 6;
        }
    }
}

// C:1 W:2 M:3 B:4 G:5 Y:6 R:7 K:8
int closePoint(int r, int g, int b, std::vector<int> c){
    std::vector<int> distances(4, 0);
    for(int i = 0; i < 4; i++){
        if(c[i] == 1){
            distances[i] = distanceSquared(r, g, b, 0, 255, 255);
        }else if(c[i] == 2){
            distances[i] = distanceSquared(r, g, b, 255, 255, 255);
        }else if(c[i] == 3){
            distances[i] = distanceSquared(r, g, b, 255, 0, 255);
        }else if(c[i] == 4){
            distances[i] = distanceSquared(r, g, b, 0, 0, 255);
        }else if(c[i] == 5){
            distances[i] = distanceSquared(r, g, b, 0, 255, 0);
        }else if(c[i] == 6){
            distances[i] = distanceSquared(r, g, b, 255, 255, 0);
        }else if(c[i] == 7){
            distances[i] = distanceSquared(r, g, b, 255, 0, 0);
        }else if(c[i] == 8){
            distances[i] = distanceSquared(r, g, b, 0, 0, 0);
        }
    }

    int min = 0;
    for(int i = 1; i < 4; i++){
        if(distances[i] < distances[min]){
            min = i;
        }
    }

    return c[min];
}

std::vector<int> getPointRGB(int index){
    if(index == 1){
        return {0, 255, 255};
    }else if(index == 2){
        return {255, 255, 255};
    }else if(index == 3){
        return {255, 0, 255};
    }else if(index == 4){
        return {0, 0, 255};
    }else if(index == 5){
        return {0, 255, 0};
    }else if(index == 6){
        return {255, 255, 0};
    }else if(index == 7){
        return {255, 0, 0};
    }else if(index == 8){
        return {0, 0, 0};
    }
    return {0,0,0};
}

void ErrorDiffusion::MBVQBasedED(std::string outputPath){
    std::vector<std::vector<std::vector<unsigned char> > > result = image;

    // Floyd-Steinberg error diffusion
    for(int i = 0; i < height; i++){
        if(i % 2 == 0){
            for(int j = 0; j < width; j++){
                // Determine MBVQ
                int MBVQ = locateMBVQ(result[i][j][0], result[i][j][1], result[i][j][2]);
                
                // calculate close point
                int cp = 0;
                if(MBVQ == 1){
                    std::vector<int> c = {1, 2, 3, 6};
                    cp = closePoint(result[i][j][0], result[i][j][1], result[i][j][2], c);                
                }else if(MBVQ == 2){
                    std::vector<int> c = {1, 3, 5, 6};
                    cp = closePoint(result[i][j][0], result[i][j][1], result[i][j][2], c);  
                }else if(MBVQ == 3){
                    std::vector<int> c = {3, 5, 6, 7};
                    cp = closePoint(result[i][j][0], result[i][j][1], result[i][j][2], c);  
                }else if(MBVQ == 4){
                    std::vector<int> c = {4, 5, 7, 8};
                    cp = closePoint(result[i][j][0], result[i][j][1], result[i][j][2], c);  
                }else if(MBVQ == 5){
                    std::vector<int> c = {3, 4, 5, 7};
                    cp = closePoint(result[i][j][0], result[i][j][1], result[i][j][2], c);  
                }else if(MBVQ == 6){
                    std::vector<int> c = {1, 3, 4, 5};
                    cp = closePoint(result[i][j][0], result[i][j][1], result[i][j][2], c);  
                }
                std::vector<int> cRGB = getPointRGB(cp);

                // error diffusion
                for(int k = 0; k < 3; k++){
                    int error = result[i][j][k] - cRGB[k];
                    result[i][j][k] = cRGB[k];

                    if (j < width - 1){
                        result[i][j + 1][k] += error * 7 / 16;
                    }
                    if (j < width - 1 && i < height - 1){
                        result[i + 1][j + 1][k] += error / 16;
                    }
                    if(i < height - 1){
                        result[i + 1][j][k] += error * 5 / 16;
                    }
                    if(i < height - 1 && j > 0){
                        result[i + 1][j - 1][k] += error * 3 / 16;
                    } 
                }      
            }
        }else{
            for(int j = width - 1; j >= 0; j--){
                // Determine MBVQ
                int MBVQ = locateMBVQ(result[i][j][0], result[i][j][1], result[i][j][2]);
                
                int cp = 0;
                if(MBVQ == 1){
                    std::vector<int> c = {1, 2, 3, 6};
                    cp = closePoint(result[i][j][0], result[i][j][1], result[i][j][2], c);                      
                }else if(MBVQ == 2){
                    std::vector<int> c = {1, 3, 5, 6};
                    cp = closePoint(result[i][j][0], result[i][j][1], result[i][j][2], c);  
                }else if(MBVQ == 3){
                    std::vector<int> c = {3, 5, 6, 7};
                    cp = closePoint(result[i][j][0], result[i][j][1], result[i][j][2], c);  
                }else if(MBVQ == 4){
                    std::vector<int> c = {4, 5, 7, 8};
                    cp = closePoint(result[i][j][0], result[i][j][1], result[i][j][2], c);  
                }else if(MBVQ == 5){
                    std::vector<int> c = {3, 4, 5, 7};
                    cp = closePoint(result[i][j][0], result[i][j][1], result[i][j][2], c);  
                }else if(MBVQ == 6){
                    std::vector<int> c = {1, 3, 4, 5};
                    cp = closePoint(result[i][j][0], result[i][j][1], result[i][j][2], c);  
                }

                std::vector<int> cRGB = getPointRGB(cp);
                for(int k = 0; k < 3; k++){
                    int error = result[i][j][k] - cRGB[k];
                    result[i][j][k] = cRGB[k];

                    if (j < width - 1 && i < height - 1){
                        result[i + 1][j + 1][k] += error * 3 / 16;
                    }
                    if(i < height - 1){
                        result[i + 1][j][k] += error * 5 / 16;
                    }
                    if(i < height - 1 && j > 0){
                        result[i + 1][j - 1][k] += error / 16;
                    } 
                    if(j > 0){
                        result[i][j - 1][k] += error * 7 / 16;
                    } 
                } 
            }
        }
    }
    
    // write image
    RWImage rwi(height, width);
    rwi.writeRawImage(outputPath, result, 3);
}
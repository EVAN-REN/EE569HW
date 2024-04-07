#include "ColorTrans.h"

ColorTrans::ColorTrans(int h, int w):height(h), width(w){

}

std::vector<std::vector<std::vector<unsigned char> > > ColorTrans::RGB2GRAY(std::vector<std::vector<std::vector<unsigned char> > > inputImage){
    std::vector<std::vector<std::vector<unsigned char> > > result(height, std::vector<std::vector<unsigned char> >(width, std::vector<unsigned char>(1, 0)));

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            // Y = 0.2989 ∗ R + 0.5870 ∗ G + 0.1140 ∗ B
            result[i][j][0] = static_cast<unsigned char>(std::floor(0.2989 * inputImage[i][j][0] + 0.5870 * inputImage[i][j][1] + 0.1140 * inputImage[i][j][2]));
        }
    }

    return result;
}
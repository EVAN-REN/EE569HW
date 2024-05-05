#include "WaterColor.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

int main(){
    std::string outputPath = "/Users/ren/Documents/GitHub/EE569HW/";
    std::string rPath = "../images/Flower.raw"; 
    std::string rPath_noise = "../images/Flower_noisy.raw"; 
    WaterColor wc(512, 768, 3);
    wc.readRawImage(rPath_noise);

    std::string wPath_Im = outputPath + "HW1/p3/resultImage/Flower_Im.raw";
    std::vector<std::vector<std::vector<unsigned char> > > Im = wc.medianFilter(wc.RGB2YUV(), 5);
    wc.writeRawImage(wPath_Im, 1, wc.YUV2RGB(Im));
    
    int k = 5;
    std::string wPath_Ib = outputPath + "HW1/p3/resultImage/Flower_Ib.raw";
    std::vector<std::vector<std::vector<unsigned char> > > Ib = Im;
    for(int i = 0; i < k; i++){
        Ib = wc.bilateralFilter(Ib, 5, 20, 10);
    }
    wc.writeRawImage(wPath_Ib, 1, wc.YUV2RGB(Ib));

    std::string wPath_Ig = outputPath + "HW1/p3/resultImage/Flower_Ig.raw";
    std::vector<std::vector<std::vector<unsigned char> > > Ig = wc.GaussianFilter(wc.RGB2YUV(), 7, 2);
    wc.writeRawImage(wPath_Ig, 1, wc.YUV2RGB(Ig));

    std::string wPath_Iout = outputPath + "HW1/p3/resultImage/Flower_Iout.raw";
    wc.writeRawImage(wPath_Iout, 1, wc.YUV2RGB(wc.Linear_Combination(Ib, 1.4, Ig, 0.4)));

    return 0;
}
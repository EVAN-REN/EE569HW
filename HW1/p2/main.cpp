#include "Basic.h"
#include "Bilateral.h"
#include "NonLocalMeans.h"
#include "ColorImage.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

int main(){
    std::string outputPath = "/Users/ren/Documents/hw/EE569/"; 
    // (a)
    Basic bas(512, 768, 1);
    std::string rPath = "../images/Flower_gray_noisy.raw"; 
    bas.readRawImage(rPath);
    std::string wBPath = outputPath + "HW1/p2/resultImage/uniform_gray.raw";
    std::string wGPath = outputPath + "HW1/p2/resultImage/gaussian_gray.raw";
    bas.writeRawImage(wBPath, 0, bas.uniformWeight());
    bas.writeRawImage(wGPath, 0, bas.GaussianWeight(3, 1));

    // (b)
    Bilateral bil(512, 768, 1);
    bil.readRawImage(rPath);
    std::string wBiPath = outputPath + "HW1/p2/resultImage/bilateral_gray.raw";
    bil.writeRawImage(wBiPath, 0, bil.BilateralFunction(7, 1, 33));

    // // (c)
    NonLocalMeans nlm(512, 768, 1);
    std::string wNPath = outputPath + "HW1/p2/resultImage/NLM_gray.raw";
    nlm.readRawImage(rPath);
    nlm.writeRawImage(wNPath, 0, nlm.NLM_Function(10, 7, 21));

    // // (d)
    ColorImage ci(512, 768, 3);
    std::string rcPath = "../images/Flower_noisy.raw";
    // std::string wMedianPath = outputPath + "HW1/p2/resultImage/median_color.raw";
    // std::string wMeanPath = outputPath + "HW1/p2/resultImage/mean_color.raw";
    // std::string wGaussianPath = outputPath + "HW1/p2/resultImage/gaussian_color.raw";
    // std::string wBilateralPath = outputPath + "HW1/p2/resultImage/bilateral_color.raw";
    // std::string wNLMPath = outputPath + "HW1/p2/resultImage/NLM_color.raw";
    std::string wCFPath = outputPath + "HW1/p2/resultImage/flower_color.raw";  
    ci.readRawImage(rcPath);
    std::vector<std::vector<std::vector<unsigned char> > > yuiImage = ci.RGB2YUV();
    ci.writeRawImage(wCFPath, 1, ci.YUV2RGB(ci.NLMFilter(ci.medianFilter(yuiImage, 3), 10, 7, 21)));
    // ci.writeRawImage(wMedianPath, 1, ci.YUV2RGB(ci.medianFilter(yuiImage, 3)));
    // ci.writeRawImage(wMeanPath, 1, ci.YUV2RGB(ci.meanFilter(yuiImage))); 
    // ci.writeRawImage(wGaussianPath, 1, ci.YUV2RGB(ci.GaussianFilter(yuiImage, 7, 1)));
    // ci.writeRawImage(wBilateralPath, 1, ci.YUV2RGB(ci.bilateralFilter(yuiImage, 7, 1, 30)));
    // ci.writeRawImage(wNLMPath, 1, ci.YUV2RGB(ci.NLMFilter(yuiImage, 10, 7, 21)));
    // std::vector<std::vector<std::vector<unsigned char> > > hsvImage = ci.RGB2HSV();
    // ci.writeRawImage(wMedianPath, 1, ci.HSV2RGB(ci.medianFilter(hsvImage, 5)));
    // ci.writeRawImage(wMeanPath, 1, ci.HSV2RGB(ci.meanFilter(hsvImage))); 
    // ci.writeRawImage(wGaussianPath, 1, ci.HSV2RGB(ci.GaussianFilter(hsvImage, 7, 1)));
    // ci.writeRawImage(wBilateralPath, 1, ci.HSV2RGB(ci.bilateralFilter(hsvImage, 7, 1, 30)));
    // ci.writeRawImage(wNLMPath, 1, ci.HSV2RGB(ci.NLMFilter(hsvImage, 10, 7, 21)));

    
    return 0;
}
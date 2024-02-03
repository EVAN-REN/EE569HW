#include "Demosaic.h"
#include "Histogram.h"
#include "Clahe.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(){
    std::string outputPath = "/Users/ren/Documents/hw/EE569/";
    // (a)
    Demosaic demo(288, 420);
    std::string rPath_D = "../images/House.raw"; 
    std::string wPath_D = outputPath + "HW1/p1/resultImage/DHouse.raw";
    demo.readRawImage(rPath_D);
    std::vector<std::vector<std::vector<unsigned char> > > grayImag = demo.mirrorReflection();
    demo.Bilinear_Demosaicing(grayImag);
    demo.writeRawImage(wPath_D);
    
    // (b)
    Histogram hist(340, 596);
    std::string rPath_H = "../images/DimLight.raw"; 
    std::string wTPath_H = outputPath + "HW1/p1/resultImage/TDimLight.raw";
    std::string wBPath_H = outputPath + "HW1/p1/resultImage/BDimLight.raw";
    hist.readRawImage(rPath_H);
    // hist.plotOrignalImage();
    hist.writeRawImage(wTPath_H, 0, hist.transferFunctionBased());
    hist.writeRawImage(wBPath_H, 0, hist.bucketFilling());

    // (c)
    Clahe clahe(422, 750);
    std::string rPath_C = "../images/City.raw"; 
    clahe.readRawImage(rPath_C);
    std::string wTPath_C = outputPath + "HW1/p1/resultImage/TCity.raw"; 
    std::string wBPath_C = outputPath + "HW1/p1/resultImage/BCity.raw";
    std::string wCPath_C = outputPath + "HW1/p1/resultImage/CCity.raw";
    clahe.writeRawImage(wTPath_C , clahe.YUV2RGB(clahe.transferFunctionBased(clahe.RGB2YUV())));
    clahe.writeRawImage(wBPath_C , clahe.YUV2RGB(clahe.bucketFilling(clahe.RGB2YUV())));
    clahe.writeRawImage(wCPath_C , clahe.YUV2RGB(clahe.cvCLAHE(clahe.RGB2YUV())));
    
    return 0;
}
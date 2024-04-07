#include "ErrorDiffusion.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

int main(){
    std::string outputPath = "/Users/ren/Documents/hw/EE569/HW2/";
    std::string BirdPath = "../images/Bird.raw";  
    ErrorDiffusion ed(BirdPath, 375, 500, 3);

    std::string BirdSEDPath = outputPath + "p3/resultImage/Bird_SED.raw";
    ed.separableED(BirdSEDPath);

    std::string BirdMBVQPath = outputPath + "p3/resultImage/Bird_MBVQ.raw";
    ed.MBVQBasedED(BirdMBVQPath);
    return 0;
}
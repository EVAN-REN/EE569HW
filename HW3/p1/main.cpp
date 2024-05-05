#include "ImageTransform.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

int main(){
    std::string outputPath = "/Users/ren/Documents/GitHub/EE569HW/HW3/";
    std::string dogPath = "../images/dog.raw";
    std::string catPath = "../images/cat.raw";

    std::string WriteForwardDogPath = outputPath + "p1/resultImage/warpDog.raw";
    std::string WritereverseDogPath = outputPath + "p1/resultImage/reverseWarpDog.raw";
    ImageTransform itfd(dogPath, 328, 328, 3);
    itfd.ForwardAddressMapping(WriteForwardDogPath);
    itfd.ReverseSpatialWarping(WritereverseDogPath);

    std::string WriteForwardCatPath = outputPath + "p1/resultImage/warpCat.raw";
    std::string WritereverseCatPath = outputPath + "p1/resultImage/reverseWarpCat.raw";
    ImageTransform itfc(catPath, 328, 328, 3);
    itfc.ForwardAddressMapping(WriteForwardCatPath);
    itfc.ReverseSpatialWarping(WritereverseCatPath);
    
    return 0;
}
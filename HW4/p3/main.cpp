#include "ImageMatching.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

int main(){
    std::string inputPath = "/Users/ren/Documents/hw/EE569/HW4/";

    ImageMatching im(inputPath, 180, 270, 3);
    // im.extract_SIFT_Features(0,2);
    // im.extract_SIFT_Features(2,1);
    // im.extract_SIFT_Features(3,2);
    // im.extract_SIFT_Features(0,3);
    im.bagOfWords();
    

    return 0;
}
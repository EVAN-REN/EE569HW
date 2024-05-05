#include "EdgeDetection.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

int main(){
    std::string outputPath = "/Users/ren/Documents/GitHub/EE569HW/HW2/";
    std::string tigerPath = "../images/Tiger.raw";
    std::string pigPath = "../images/Pig.raw";

    EdgeDetection tigerSED(tigerPath, 321, 481, 3);
    std::string tigerSWPath = outputPath + "p1/resultImages/Tiger";
    // sobel edge detect
    tigerSED.sobelED(tigerSWPath, 0.1);

    std::string pigSWPath = outputPath + "p1/resultImages/Pig";
    EdgeDetection pigSED(pigPath, 321, 481, 3);
    pigSED.sobelED(pigSWPath, 0.1);

    EdgeDetection tigerCED(tigerPath, 321, 481, 3);
    std::string tigerCWPath = outputPath + "p1/resultImages/Tiger";
    // canny edge detect(output, lowerThreshold, upperThreshold, windowsize)
    tigerCED.cannyED(tigerCWPath, 150, 200, 3);

    std::string pigCWPath = outputPath + "p1/resultImages/Pig";
    EdgeDetection pigCED(pigPath, 321, 481, 3);
    pigCED.cannyED(pigCWPath, 100, 150, 3);


    
    return 0;
}
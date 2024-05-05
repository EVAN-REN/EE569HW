#include "HomographicTrans.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

int main(){
    std::string outputPath = "/Users/ren/Documents/GitHub/EE569HW/HW3/";
    std::string leftToysPath = "../images/toys_left.raw";
    std::string middleToysPath = "../images/toys_middle.raw";
    std::string rightToysPath = "../images/toys_right.raw";

    std::string WriteToysPath = outputPath + "p2/resultImage/toys.raw";
    // Homographic Transformation
    HomographicTrans ht(leftToysPath, middleToysPath, rightToysPath, 454, 605, 3);
    ht.SURF_Method(WriteToysPath);
    return 0;
}
#include "HalfToned.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

int main(){
    std::string outputPath = "/Users/ren/Documents/hw/EE569/HW2/";
    std::string LightHousePath = "../images/LightHouse.raw";
    HalfToned ht(LightHousePath, 500, 750, 1);

    std::string fixedTHPath = outputPath + "p2/resultImage/LightHouse_fixed.raw";
    ht.fixedThresholding(fixedTHPath, 0.5);

    std::string randomTHPath = outputPath + "p2/resultImage/LightHouse_random.raw";
    ht.randomThresholding(randomTHPath);
    
    std::string matrixTHPath2 = outputPath + "p2/resultImage/LightHouse_matrix_2.raw";
    ht.ditheringMatrix(matrixTHPath2, 2);

    std::string matrixTHPath8 = outputPath + "p2/resultImage/LightHouse_matrix_8.raw";
    ht.ditheringMatrix(matrixTHPath8, 8);

    std::string matrixTHPath32 = outputPath + "p2/resultImage/LightHouse_matrix_32.raw";
    ht.ditheringMatrix(matrixTHPath32, 32);

    std::string EDFSPath = outputPath + "p2/resultImage/LightHouse_ED_FS.raw";
    ht.errorDiffusion_FS(EDFSPath);

    std::string EDJJNPath = outputPath + "p2/resultImage/LightHouse_ED_JJN.raw";
    ht.errorDiffusion_JJN(EDJJNPath);

    std::string EDSTUPath = outputPath + "p2/resultImage/LightHouse_ED_STU.raw";
    ht.errorDiffusion_Stu(EDSTUPath);

    
    return 0;
}
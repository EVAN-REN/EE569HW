#include "TextureAnalysis.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

int main(){
    std::string outputPath = "/Users/ren/Documents/GitHub/EE569HW/HW4/";
    std::string inputPath = "../";

    TextureAnalysis ta(inputPath, 128, 128,1);
    ta.nearestNeighborRuleClassifier();
    ta.K_meansClassifier();
    ta.SVMaClassifier();
    
    return 0;
}
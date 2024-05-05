#include "TextureSegmentation.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

int main(){
    std::string inputPath = "../images/composite.png";

    TextureSegmentation ts(inputPath, 512, 512,1);
    ts.basicMethod();
    ts.advancedMethod();

    return 0;
}
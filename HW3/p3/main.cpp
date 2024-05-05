#include "MorphologyProc.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

int main(){
    std::string outputPath = "/Users/ren/Documents/GitHub/EE569HW/HW3/";
    std::string FlowerPath = "../images/flower.raw";
    std::string JarPath = "../images/jar.raw"; 
    std::string SpringPath = "../images/spring.raw"; 
    
    // shrinking image (20th and final)
    MorphologyProc mp1(FlowerPath, 247, 247, 1);
    std::string FlowerThinPath = outputPath + "p3/resultImage/flower_thin_";
    mp1.basic_morphological_process(FlowerThinPath);

    MorphologyProc mp2(JarPath, 252, 252, 1);
    std::string JarThinPath = outputPath + "p3/resultImage/jar_thin_";
    mp2.basic_morphological_process(JarThinPath);

    MorphologyProc mp3(SpringPath, 252, 252, 1);
    std::string SpringThinPath = outputPath + "p3/resultImage/spring_thin_";
    mp3.basic_morphological_process(SpringThinPath);

    // shape detect
    std::string BoardPath = "../images/board.raw";
    MorphologyProc mp4(BoardPath, 768, 768, 1);
    std::string BoardOutputPath1 = outputPath + "p3/resultImage/board_1.raw";
    std::string BoardOutputPath2 = outputPath + "p3/resultImage/board_2.raw";
    mp4.shape_detection1(BoardOutputPath1);
    mp4.shape_detection2(BoardOutputPath2);
    MorphologyProc mp5(BoardPath, 768, 768, 1);
    std::string BoardOutputPath3 = outputPath + "p3/resultImage/board_3.raw";
    mp5.shape_detection3(BoardOutputPath3);

    std::string BeanPath = "../images/beans.raw";
    MorphologyProc mp6(BeanPath, 82, 494, 3);
    std::string BeanOutputPath = outputPath + "p3/resultImage/beans_output.raw";
    mp6.object_segmentation(BeanOutputPath, 0.85);

    return 0;
}
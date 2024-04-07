#ifndef TEXTUREANALYSIS_H
#define TEXTUREANALYSIS_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;

class TextureAnalysis{
    private:
        int height;
        int width;
        int BytesPerPixel;
        std::vector<Mat> train_image;
        std::vector<Mat> test_image;
        // 2 1 4 1 4 3 3 4 2 2 1 3
        // 1-blanket 2-brick 3-grass 4-stones
        std::vector<int> train_labels;
        std::vector<int> test_labels;

    public:
        TextureAnalysis(){}

        TextureAnalysis(std::string path, int h, int w, int b);
        void nearestNeighborRuleClassifier();
        void K_meansClassifier();
        void SVMaClassifier();
        void calculateDiscriminantPower(std::vector<Mat> &FeatureVectors);

};


#endif // TEXTUREANALYSIS_H
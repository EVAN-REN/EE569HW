#ifndef TEXTURESEGMENTATION_H
#define TEXTURESEGMENTATION_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;

class TextureSegmentation{
    private:
        int height;
        int width;
        int BytesPerPixel;
        Mat image;
    
    public:
        TextureSegmentation(){}

        TextureSegmentation(std::string path, int h, int w, int b);
        void basicMethod();
        void advancedMethod();
};


#endif // TEXTURESEGMENTATION_H
#ifndef RWIMAGE_H
#define RWIMAGE_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;

class RWImage{
    private:
        int height;
        int width;

    public:
        RWImage(){}

        RWImage(int h, int w);
        Mat readRawImage(std::string path, int BytesPerPixel);
        void writeRawImage(std::string path, Mat inputImage, int BytesPerPixel);

};


#endif // RWIMAGE_H
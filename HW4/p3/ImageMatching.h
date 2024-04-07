#ifndef MORPHOLOGYPROC_H
#define MORPHOLOGYPROC_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;

class ImageMatching{
    private:
        int height;
        int width;
        int BytesPerPixel;
        std::vector<Mat> images;

    public:
        ImageMatching(){}

        ImageMatching(std::string path, int h, int w, int b);
        void extract_SIFT_Features(int i1, int i2);
        void bagOfWords();
};


#endif // MORPHOLOGYPROC_H  
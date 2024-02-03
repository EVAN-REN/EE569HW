#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>


class Histogram{
    private:
        std::vector<std::vector<std::vector<unsigned char> > > image;
        int BytesPerPixel;
        int height;
        int width;

    public:
        Histogram(){}

        Histogram(int h, int w);
        void readRawImage(std::string path);
        void writeRawImage(std::string path, int type, std::vector<std::vector<std::vector<unsigned char> > > image);
        void writeRawImage(std::string path);
        void plotOrignalImage();
        std::vector<std::vector<std::vector<unsigned char> > > transferFunctionBased();
        std::vector<std::vector<std::vector<unsigned char> > > bucketFilling();
};


#endif // HISTOGRAM_H
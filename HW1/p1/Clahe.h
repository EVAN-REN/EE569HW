#ifndef CLAHE_H
#define CLAHE_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

class Clahe{
    private:
        std::vector<std::vector<std::vector<unsigned char> > > image;
        int BytesPerPixel;
        int height;
        int width;

    public:
        Clahe(){}

        Clahe(int h, int w);
        void readRawImage(std::string path);
        void writeRawImage(std::string path, std::vector<std::vector<std::vector<unsigned char> > > inputImage);
        std::vector<std::vector<std::vector<unsigned char> > > RGB2YUV();
        std::vector<std::vector<std::vector<unsigned char> > > transferFunctionBased(std::vector<std::vector<std::vector<unsigned char> > > inputImage);
        std::vector<std::vector<std::vector<unsigned char> > > bucketFilling(std::vector<std::vector<std::vector<unsigned char> > > inputImage);
        std::vector<std::vector<std::vector<unsigned char> > > YUV2RGB(std::vector<std::vector<std::vector<unsigned char> > > inputImage);
        std::vector<std::vector<std::vector<unsigned char> > > cvCLAHE(std::vector<std::vector<std::vector<unsigned char> > > inputImage);
};


#endif // CLAHE_H
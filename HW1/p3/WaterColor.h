#ifndef WATERCOLOR_H
#define WATERCOLOR_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

class WaterColor{
    private:
        std::vector<std::vector<std::vector<unsigned char> > > image;
        int BytesPerPixel;
        int height;
        int width;

    public:
        WaterColor(){}

        WaterColor(int h, int w, int b);
        void readRawImage(std::string path);
        void writeRawImage(std::string path, int type, std::vector<std::vector<std::vector<unsigned char> > > wImage);
        std::vector<std::vector<std::vector<unsigned char> > >  GaussianFilter(std::vector<std::vector<std::vector<unsigned char> > > inputImage, int lenSide, double sigma);
        std::vector<std::vector<std::vector<unsigned char> > >  bilateralFilter(std::vector<std::vector<std::vector<unsigned char> > > inputImage, int lenSide, double sigmac, double sigmas);
        std::vector<std::vector<std::vector<unsigned char> > >  medianFilter(std::vector<std::vector<std::vector<unsigned char> > > inputImage, int windowSize);
        std::vector<std::vector<std::vector<unsigned char> > > RGB2YUV();
        std::vector<std::vector<std::vector<unsigned char> > > YUV2RGB(std::vector<std::vector<std::vector<unsigned char> > > inputImage);
        double calculatePSNR(std::vector<std::vector<std::vector<unsigned char> > > compareImage, int channel);
        std::vector<std::vector<std::vector<unsigned char> > >  Linear_Combination(std::vector<std::vector<std::vector<unsigned char> > > inputImage1, double x1, std::vector<std::vector<std::vector<unsigned char> > > inputImage2, double x2);
};

#endif // WATERCOLOR_H
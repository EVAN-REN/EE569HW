#ifndef COLORIMAGE_H
#define COLORIMAGE_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

class ColorImage{
    private:
        std::vector<std::vector<std::vector<unsigned char> > > image;
        int BytesPerPixel;
        int height;
        int width;

    public:
        ColorImage(){}

        ColorImage(int h, int w, int b);
        void readRawImage(std::string path);
        void writeRawImage(std::string path, int type, std::vector<std::vector<std::vector<unsigned char> > > wImage);
        std::vector<std::vector<std::vector<unsigned char> > >  meanFilter(std::vector<std::vector<std::vector<unsigned char> > > inputImage);
        std::vector<std::vector<std::vector<unsigned char> > >  GaussianFilter(std::vector<std::vector<std::vector<unsigned char> > > inputImage, int lenSide, double sigma);
        std::vector<std::vector<std::vector<unsigned char> > >  bilateralFilter(std::vector<std::vector<std::vector<unsigned char> > > inputImage, int lenSide, double sigmac, double sigmas);
        std::vector<std::vector<std::vector<unsigned char> > >  NLMFilter(std::vector<std::vector<std::vector<unsigned char> > > inputImage, float intensity, int templateWindowSize, int searchWindowSize);
        std::vector<std::vector<std::vector<unsigned char> > >  medianFilter(std::vector<std::vector<std::vector<unsigned char> > > inputImage, int windowSize);
        std::vector<std::vector<std::vector<unsigned char> > > RGB2YUV();
        std::vector<std::vector<std::vector<unsigned char> > > YUV2RGB(std::vector<std::vector<std::vector<unsigned char> > > inputImage);
        std::vector<std::vector<std::vector<unsigned char> > > RGB2HSV();
        std::vector<std::vector<std::vector<unsigned char> > > HSV2RGB(std::vector<std::vector<std::vector<unsigned char> > > inputImage);
        double calculatePSNR(std::vector<std::vector<std::vector<unsigned char> > > compareImage, int channel);
};

#endif // COLORIMAGE_H
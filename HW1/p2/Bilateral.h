#ifndef BILATERAL_H
#define BILATERAL_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

class Bilateral{
    private:
        std::vector<std::vector<std::vector<unsigned char> > > image;
        int BytesPerPixel;
        int height;
        int width;

    public:
        Bilateral(){}

        Bilateral(int h, int w, int b);
        void readRawImage(std::string path);
        void writeRawImage(std::string path, int type, std::vector<std::vector<std::vector<unsigned char> > > wImage);
        std::vector<std::vector<std::vector<unsigned char> > >  BilateralFunction(int lenSide, double sigmac, double sigmas);
        double BilateralWeight(int x, int y, int x0, int y0, double sigmac, double sigmas);
        double calculatePSNR(std::vector<std::vector<std::vector<unsigned char> > > compareImage, int channel);
        
        
};

#endif // BILATERAL_H
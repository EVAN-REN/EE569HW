#ifndef BASIC_H
#define BASIC_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

class Basic{
    private:
        std::vector<std::vector<std::vector<unsigned char> > > image;
        int BytesPerPixel;
        int height;
        int width;

    public:
        Basic(){}

        Basic(int h, int w, int b);
        void readRawImage(std::string path);
        void writeRawImage(std::string path, int type, std::vector<std::vector<std::vector<unsigned char> > > wImage);
        std::vector<std::vector<std::vector<unsigned char> > >  uniformWeight();
        std::vector<std::vector<std::vector<unsigned char> > >  GaussianWeight(int lenSide, double sigma);
        double calculatePSNR(std::vector<std::vector<std::vector<unsigned char> > > compareImage, int channel);
        
};

#endif // BASIC_H
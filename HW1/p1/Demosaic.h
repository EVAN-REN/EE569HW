#ifndef DEMOSAIC_H
#define DEMOSAIC_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

class Demosaic{
    private:
        std::vector<std::vector<std::vector<unsigned char> > > image;
        int BytesPerPixel;
        int height;
        int width;

    public:
        Demosaic(){}

        Demosaic(int h, int w);
        void readRawImage(std::string path);
        void writeRawImage(std::string path);
        std::vector<std::vector<std::vector<unsigned char> > > zeroPadding();
        std::vector<std::vector<std::vector<unsigned char> > > mirrorReflection();
        void Bilinear_Demosaicing(std::vector<std::vector<std::vector<unsigned char> > > oriImage);
};


#endif // DEMOSAIC_H
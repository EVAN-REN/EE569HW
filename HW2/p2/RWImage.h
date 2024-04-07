#ifndef RWIMAGE_H
#define RWIMAGE_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

class RWImage{
    private:
        int height;
        int width;

    public:
        RWImage(){}

        RWImage(int h, int w);
        std::vector<std::vector<std::vector<unsigned char> > > readRawImage(std::string path, int BytesPerPixel);
        void writeRawImage(std::string path, std::vector<std::vector<std::vector<unsigned char> > > inputImage, int BytesPerPixel);
};


#endif // RWIMAGE_H
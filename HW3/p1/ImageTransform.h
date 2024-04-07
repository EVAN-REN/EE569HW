#ifndef IMAGETRANSFORM_H
#define IMAGETRANSFORM_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

class ImageTransform{
    private:
        int height;
        int width;
        int BytesPerPixel;
        std::vector<std::vector<std::vector<unsigned char> > > image;

    public:
        ImageTransform(){}

        ImageTransform(std::string path, int h, int w, int b);
        void ForwardAddressMapping(std::string outputPath);
        void InverseAddressMapping(std::string outputPath);
        void ReverseSpatialWarping(std::string outputPath);
        std::vector<int> bilinearInterpolation(double x, double y);
};


#endif // IMAGETRANSFORM_H
#ifndef ERRORDIFFUSION_H
#define ERRORDIFFUSION_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

class ErrorDiffusion{
    private:
        int height;
        int width;
        int BytesPerPixel;
        std::vector<std::vector<std::vector<unsigned char> > > image;

    public:
        ErrorDiffusion(){}

        ErrorDiffusion(std::string path, int h, int w, int b);
        void separableED(std::string outputPath);
        void MBVQBasedED(std::string outputPath);
};


#endif // ERRORDIFFUSION_H  
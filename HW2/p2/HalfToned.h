#ifndef HALFTONED_H
#define HALFTONED_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

class HalfToned{
    private:
        int height;
        int width;
        int BytesPerPixel;
        std::vector<std::vector<std::vector<unsigned char> > > image;

    public:
        HalfToned(){}

        HalfToned(std::string path, int h, int w, int b);
        void fixedThresholding(std::string outputPath, double threshold);
        void randomThresholding(std::string outputPath);
        void ditheringMatrix(std::string outputPath, int matrixSize);
        void errorDiffusion_FS(std::string outputPath);
        void errorDiffusion_JJN(std::string outputPath);
        void errorDiffusion_Stu(std::string outputPath);
};


#endif // HALFTONED_H
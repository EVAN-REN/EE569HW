#ifndef NONLOCALMEANS_H
#define NONLOCALMEANS_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

class NonLocalMeans{
    private:
        std::vector<std::vector<std::vector<unsigned char> > > image;
        int BytesPerPixel;
        int height;
        int width;

    public:
        NonLocalMeans(){}

        NonLocalMeans(int h, int w, int b);
        void readRawImage(std::string path);
        void writeRawImage(std::string path, int type, std::vector<std::vector<std::vector<unsigned char> > > wImage);
        std::vector<std::vector<std::vector<unsigned char> > >  NLM_Function(float intensity, int templateWindowSize, int searchWindowSize);
        double calculatePSNR(std::vector<std::vector<std::vector<unsigned char> > > compareImage, int channel);
        
};

#endif // NONLOCALMEANS_H
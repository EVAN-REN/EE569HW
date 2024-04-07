#ifndef EDGEDETECTION_H
#define EDGEDETECTION_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

class EdgeDetection{
    private:
        int height;
        int width;
        int BytesPerPixel;
        std::vector<std::vector<std::vector<unsigned char> > > image;

    public:
        EdgeDetection(){}

        EdgeDetection(std::string path, int h, int w, int b);
        void sobelED(std::string outputPath, double threshold);
        void cannyED(std::string outputPath, double lowerThreshold, double upperThreshold, int windowSize);
    
};


#endif // EDGEDETECTION_H
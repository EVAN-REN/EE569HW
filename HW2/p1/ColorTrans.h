#ifndef COLORTRANS_H
#define COLORTRANS_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

class ColorTrans{
    private:
        int height;
        int width;

    public:
        ColorTrans(){}

        ColorTrans(int h, int w);
        std::vector<std::vector<std::vector<unsigned char> > > RGB2GRAY(std::vector<std::vector<std::vector<unsigned char> > > inputImage);
};


#endif // COLORTRANS_H
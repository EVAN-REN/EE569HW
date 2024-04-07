#ifndef HOMOGRAPHICTRANS_H
#define HOMOGRAPHICTRANS_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

class HomographicTrans{
    private:
        int height;
        int width;
        int BytesPerPixel;
        std::vector<std::vector<std::vector<unsigned char> > > image_left;
        std::vector<std::vector<std::vector<unsigned char> > > image_middle;
        std::vector<std::vector<std::vector<unsigned char> > > image_right;
    
    public:
        HomographicTrans(){}

        HomographicTrans(std::string path1, std::string path2, std::string path3, int h, int w, int b);
        void SURF_Method(std::string outputPath);
};


#endif // HOMOGRAPHICTRANS_H
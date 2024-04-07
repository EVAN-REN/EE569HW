#ifndef MORPHOLOGYPROC_H
#define MORPHOLOGYPROC_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

class MorphologyProc{
    private:
        int height;
        int width;
        int BytesPerPixel;
        std::vector<std::vector<std::vector<unsigned char> > > image;

    public:
        MorphologyProc(){}

        MorphologyProc(std::string path, int h, int w, int b);
        void zero_padding();
        void de_zero_padding();
        void binarize();
        bool isConditionalPatternMatch(int x, int y, int type);
        bool isUnconditionalPatternMatch(int x, int y, std::vector<std::vector<unsigned char> >  &M);
        void basic_morphological_process(std::string outputPath);

        void shape_detection1(std::string outputPath);
        void shape_detection2(std::string outputPath);
        void shape_detection3(std::string outputPath);
        bool ifDot(int x, int y);
        std::vector<int> AllConnectedArea();
        void DFS_helper(int x, int y, std::vector<std::vector<bool> >  &record, std::vector<int> &counts);
        void fillHole(int x, int y, std::vector<std::vector<std::vector<unsigned char> > > &inputImage);

        void object_segmentation(std::string outputPath, float threshold);
};


#endif // MORPHOLOGYPROC_H  
#include "ImageTransform.h"
#include "RWImage.h"
#include <Eigen/Dense>
#include <cmath>


ImageTransform::ImageTransform(std::string path, int h, int w, int b):height(h), width(w), BytesPerPixel(b){
    RWImage rwi(h, w);
    image = rwi.readRawImage(path, b);
}

Eigen::Matrix<double, 2, 6> getAffineMatrix(double srcPoints_x[6], double srcPoints_y[6], double dstPoints_x[6], double dstPoints_y[6]){
    Eigen::Matrix<double, 6, 6> X;
    for (int i = 0; i < 6; ++i) {
        double x = srcPoints_x[i];
        double y = srcPoints_y[i];
        X.row(i) << 1, x, y, x * x, x * y, y * y;
    }

    Eigen::Matrix<double, 2, 6> P;
    P.row(0) << dstPoints_x[0], dstPoints_x[1], dstPoints_x[2], dstPoints_x[3], dstPoints_x[4], dstPoints_x[5];
    P.row(1) << dstPoints_y[0], dstPoints_y[1], dstPoints_y[2], dstPoints_y[3], dstPoints_y[4], dstPoints_y[5];

     Eigen::Matrix<double, 2, 6> C = P * X.transpose().inverse();

     return C;
}

void ImageTransform::ForwardAddressMapping(std::string outputPath){
    RWImage rwi(height, width);

    Eigen::Matrix<double, 2, 6> affineMatrix_left, affineMatrix_right, affineMatrix_top, affineMatrix_down;
    int h = height - 1;
    int w = width - 1;

    double srcPoints_x[6] = {0, h / 2, h, h / 4, h * 3 / 4, h / 2};
    double srcPoints_y[6] = {0, 0, 0, w / 4, w / 4, w / 2};
    double dstPoints_x[6] = {0, h / 2, h, h / 4, h * 3 / 4, h / 2};
    double dstPoints_y[6] = {0, 64, 0, w / 4, w / 4, w / 2};
    affineMatrix_left = getAffineMatrix(srcPoints_x, srcPoints_y, dstPoints_x, dstPoints_y);

    srcPoints_x[0] = h;
    srcPoints_y[0] = 0;
    srcPoints_x[1] = h;
    srcPoints_y[1] = w / 2;
    srcPoints_x[2] = h;
    srcPoints_y[2] = w;
    srcPoints_x[3] = h * 3 / 4;
    srcPoints_y[3] = w / 4;
    srcPoints_x[4] = h * 3 / 4;
    srcPoints_y[4] = w * 3 / 4;
    srcPoints_x[5] = h / 2;
    srcPoints_y[5] = w / 2;
    dstPoints_x[0] = h;
    dstPoints_y[0] = 0;
    dstPoints_x[1] = h - 64;
    dstPoints_y[1] = w / 2;
    dstPoints_x[2] = h;
    dstPoints_y[2] = w;
    dstPoints_x[3] = h * 3 / 4;
    dstPoints_y[3] = w / 4;
    dstPoints_x[4] = h * 3 / 4;
    dstPoints_y[4] = w * 3 / 4;
    dstPoints_x[5] = h / 2;
    dstPoints_y[5] = w / 2;
    affineMatrix_down = getAffineMatrix(srcPoints_x, srcPoints_y, dstPoints_x, dstPoints_y);

    srcPoints_x[0] = 0;
    srcPoints_y[0] = w;
    srcPoints_x[1] = h / 2;
    srcPoints_y[1] = w;
    srcPoints_x[2] = h;
    srcPoints_y[2] = w;
    srcPoints_x[3] = h / 4;
    srcPoints_y[3] = w * 3 / 4;
    srcPoints_x[4] = h * 3 / 4;
    srcPoints_y[4] = w * 3 / 4;
    srcPoints_x[5] = h / 2;
    srcPoints_y[5] = w / 2;
    dstPoints_x[0] = 0;
    dstPoints_y[0] = w;
    dstPoints_x[1] = h / 2;
    dstPoints_y[1] = w - 64;
    dstPoints_x[2] = h;
    dstPoints_y[2] = w;
    dstPoints_x[3] = h / 4;
    dstPoints_y[3] = w * 3 / 4;
    dstPoints_x[4] = h * 3 / 4;
    dstPoints_y[4] = w * 3 / 4;
    dstPoints_x[5] = h / 2;
    dstPoints_y[5] = w / 2;
    affineMatrix_right = getAffineMatrix(srcPoints_x, srcPoints_y, dstPoints_x, dstPoints_y);

    srcPoints_x[0] = 0;
    srcPoints_y[0] = 0;
    srcPoints_x[1] = 0;
    srcPoints_y[1] = w / 2;
    srcPoints_x[2] = 0;
    srcPoints_y[2] = w;
    srcPoints_x[3] = h / 4;
    srcPoints_y[3] = w / 4;
    srcPoints_x[4] = h / 4;
    srcPoints_y[4] = w * 3 / 4;
    srcPoints_x[5] = h / 2;
    srcPoints_y[5] = w / 2;
    dstPoints_x[0] = 0;
    dstPoints_y[0] = 0;
    dstPoints_x[1] = 64;
    dstPoints_y[1] = w / 2;
    dstPoints_x[2] = 0;
    dstPoints_y[2] = w;
    dstPoints_x[3] = h / 4;
    dstPoints_y[3] = w / 4;
    dstPoints_x[4] = h / 4;
    dstPoints_y[4] = w * 3 / 4;
    dstPoints_x[5] = h / 2;
    dstPoints_y[5] = w / 2;
    affineMatrix_top = getAffineMatrix(srcPoints_x, srcPoints_y, dstPoints_x, dstPoints_y);


    std::vector<std::vector<std::vector<unsigned char> > > resultImage(height, std::vector<std::vector<unsigned char> >(width, std::vector<unsigned char>(3, 0)));
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(i > j && j < (h - i)){
                Eigen::Matrix<double, 6, 1> op;
                op.row(0) << 1;
                op.row(1) << i;
                op.row(2) << j;
                op.row(3) << (i * i);
                op.row(4) << (i * j);
                op.row(5) << (j * j);
                Eigen::Matrix<double, 2, 1> rp = affineMatrix_left * op;
                int x = round(rp(0,0));
                int y = round(rp(1,0));
                resultImage[x][y][0] = image[i][j][0];
                resultImage[x][y][1] = image[i][j][1];
                resultImage[x][y][2] = image[i][j][2];
            }else if(i > j && j > (h - i)){
                Eigen::Matrix<double, 6, 1> op;
                op.row(0) << 1;
                op.row(1) << i;
                op.row(2) << j;
                op.row(3) << (i * i);
                op.row(4) << (i * j);
                op.row(5) << (j * j);
                Eigen::Matrix<double, 2, 1> rp = affineMatrix_down * op;
                int x = round(rp(0,0));
                int y = round(rp(1,0));
                resultImage[x][y][0] = image[i][j][0];
                resultImage[x][y][1] = image[i][j][1];
                resultImage[x][y][2] = image[i][j][2];
            }else if(i < j && j > (h - i)){
                Eigen::Matrix<double, 6, 1> op;
                op.row(0) << 1;
                op.row(1) << i;
                op.row(2) << j;
                op.row(3) << (i * i);
                op.row(4) << (i * j);
                op.row(5) << (j * j);
                Eigen::Matrix<double, 2, 1> rp = affineMatrix_right * op;
                int x = round(rp(0,0));
                int y = round(rp(1,0));
                resultImage[x][y][0] = image[i][j][0];
                resultImage[x][y][1] = image[i][j][1];
                resultImage[x][y][2] = image[i][j][2];
            }else if(i < j && j < (h - i)){
                Eigen::Matrix<double, 6, 1> op;
                op.row(0) << 1;
                op.row(1) << i;
                op.row(2) << j;
                op.row(3) << (i * i);
                op.row(4) << (i * j);
                op.row(5) << (j * j);
                Eigen::Matrix<double, 2, 1> rp = affineMatrix_top * op;
                int x = round(rp(0,0));
                int y = round(rp(1,0));
                resultImage[x][y][0] = image[i][j][0];
                resultImage[x][y][1] = image[i][j][1];
                resultImage[x][y][2] = image[i][j][2];
            }else if(i == j || j == (h - i)){
                resultImage[i][j][0] = image[i][j][0];
                resultImage[i][j][1] = image[i][j][1];
                resultImage[i][j][2] = image[i][j][2];
            }
        }
    }
    image = resultImage;
    rwi.writeRawImage(outputPath, resultImage, 3);
     
}


void ImageTransform::InverseAddressMapping(std::string outputPath){
    RWImage rwi(height, width);

    Eigen::Matrix<double, 2, 6> affineMatrix_left, affineMatrix_right, affineMatrix_top, affineMatrix_down;
    int h = height - 1;
    int w = width - 1;

    double dstPoints_x[6] = {0, h / 2, h, h / 4, h * 3 / 4, h / 2};
    double dstPoints_y[6] = {0, 0, 0, w / 4, w / 4, w / 2};
    double srcPoints_x[6] = {0, h / 2, h, h / 4, h * 3 / 4, h / 2};
    double srcPoints_y[6] = {0, 64, 0, w / 4, w / 4, w / 2};
    affineMatrix_left = getAffineMatrix(srcPoints_x, srcPoints_y, dstPoints_x, dstPoints_y);

    dstPoints_x[0] = h;
    dstPoints_y[0] = 0;
    dstPoints_x[1] = h;
    dstPoints_y[1] = w / 2;
    dstPoints_x[2] = h;
    dstPoints_y[2] = w;
    dstPoints_x[3] = h * 3 / 4;
    dstPoints_y[3] = w / 4;
    dstPoints_x[4] = h * 3 / 4;
    dstPoints_y[4] = w * 3 / 4;
    dstPoints_x[5] = h / 2;
    dstPoints_y[5] = w / 2;
    srcPoints_x[0] = h;
    srcPoints_y[0] = 0;
    srcPoints_x[1] = h - 64;
    srcPoints_y[1] = w / 2;
    srcPoints_x[2] = h;
    srcPoints_y[2] = w;
    srcPoints_x[3] = h * 3 / 4;
    srcPoints_y[3] = w / 4;
    srcPoints_x[4] = h * 3 / 4;
    srcPoints_y[4] = w * 3 / 4;
    srcPoints_x[5] = h / 2;
    srcPoints_y[5] = w / 2;
    affineMatrix_down = getAffineMatrix(srcPoints_x, srcPoints_y, dstPoints_x, dstPoints_y);

    dstPoints_x[0] = 0;
    dstPoints_y[0] = w;
    dstPoints_x[1] = h / 2;
    dstPoints_y[1] = w;
    dstPoints_x[2] = h;
    dstPoints_y[2] = w;
    dstPoints_x[3] = h / 4;
    dstPoints_y[3] = w * 3 / 4;
    dstPoints_x[4] = h * 3 / 4;
    dstPoints_y[4] = w * 3 / 4;
    dstPoints_x[5] = h / 2;
    dstPoints_y[5] = w / 2;
    srcPoints_x[0] = 0;
    srcPoints_y[0] = w;
    srcPoints_x[1] = h / 2;
    srcPoints_y[1] = w - 64;
    srcPoints_x[2] = h;
    srcPoints_y[2] = w;
    srcPoints_x[3] = h / 4;
    srcPoints_y[3] = w * 3 / 4;
    srcPoints_x[4] = h * 3 / 4;
    srcPoints_y[4] = w * 3 / 4;
    srcPoints_x[5] = h / 2;
    srcPoints_y[5] = w / 2;
    affineMatrix_right = getAffineMatrix(srcPoints_x, srcPoints_y, dstPoints_x, dstPoints_y);

    dstPoints_x[0] = 0;
    dstPoints_y[0] = 0;
    dstPoints_x[1] = 0;
    dstPoints_y[1] = w / 2;
    dstPoints_x[2] = 0;
    dstPoints_y[2] = w;
    dstPoints_x[3] = h / 4;
    dstPoints_y[3] = w / 4;
    dstPoints_x[4] = h / 4;
    dstPoints_y[4] = w * 3 / 4;
    dstPoints_x[5] = h / 2;
    dstPoints_y[5] = w / 2;
    srcPoints_x[0] = 0;
    srcPoints_y[0] = 0;
    srcPoints_x[1] = 64;
    srcPoints_y[1] = w / 2;
    srcPoints_x[2] = 0;
    srcPoints_y[2] = w;
    srcPoints_x[3] = h / 4;
    srcPoints_y[3] = w / 4;
    srcPoints_x[4] = h / 4;
    srcPoints_y[4] = w * 3 / 4;
    srcPoints_x[5] = h / 2;
    srcPoints_y[5] = w / 2;
    affineMatrix_top = getAffineMatrix(srcPoints_x, srcPoints_y, dstPoints_x, dstPoints_y);


    std::vector<std::vector<std::vector<unsigned char> > > resultImage(height, std::vector<std::vector<unsigned char> >(width, std::vector<unsigned char>(3, 0)));
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(i > j && j < (h - i)){
                Eigen::Matrix<double, 6, 1> op;
                op.row(0) << 1;
                op.row(1) << i;
                op.row(2) << j;
                op.row(3) << (i * i);
                op.row(4) << (i * j);
                op.row(5) << (j * j);
                Eigen::Matrix<double, 2, 1> rp = affineMatrix_left * op;
                int x = round(rp(0,0));
                int y = round(rp(1,0));
                resultImage[x][y][0] = image[i][j][0];
                resultImage[x][y][1] = image[i][j][1];
                resultImage[x][y][2] = image[i][j][2];
            }else if(i > j && j > (h - i)){
                Eigen::Matrix<double, 6, 1> op;
                op.row(0) << 1;
                op.row(1) << i;
                op.row(2) << j;
                op.row(3) << (i * i);
                op.row(4) << (i * j);
                op.row(5) << (j * j);
                Eigen::Matrix<double, 2, 1> rp = affineMatrix_down * op;
                int x = round(rp(0,0));
                int y = round(rp(1,0));
                resultImage[x][y][0] = image[i][j][0];
                resultImage[x][y][1] = image[i][j][1];
                resultImage[x][y][2] = image[i][j][2];
            }else if(i < j && j > (h - i)){
                Eigen::Matrix<double, 6, 1> op;
                op.row(0) << 1;
                op.row(1) << i;
                op.row(2) << j;
                op.row(3) << (i * i);
                op.row(4) << (i * j);
                op.row(5) << (j * j);
                Eigen::Matrix<double, 2, 1> rp = affineMatrix_right * op;
                int x = round(rp(0,0));
                int y = round(rp(1,0));
                resultImage[x][y][0] = image[i][j][0];
                resultImage[x][y][1] = image[i][j][1];
                resultImage[x][y][2] = image[i][j][2];
            }else if(i < j && j < (h - i)){
                Eigen::Matrix<double, 6, 1> op;
                op.row(0) << 1;
                op.row(1) << i;
                op.row(2) << j;
                op.row(3) << (i * i);
                op.row(4) << (i * j);
                op.row(5) << (j * j);
                Eigen::Matrix<double, 2, 1> rp = affineMatrix_top * op;
                int x = round(rp(0,0));
                int y = round(rp(1,0));
                resultImage[x][y][0] = image[i][j][0];
                resultImage[x][y][1] = image[i][j][1];
                resultImage[x][y][2] = image[i][j][2];
            }else if(i == j || j == (h - i)){
                resultImage[i][j][0] = image[i][j][0];
                resultImage[i][j][1] = image[i][j][1];
                resultImage[i][j][2] = image[i][j][2];
            }
        }
    }
    image = resultImage;
    rwi.writeRawImage(outputPath, resultImage, 3);
     
}

std::vector<int> ImageTransform::bilinearInterpolation(double x, double y) {
    int x0 = static_cast<int>(x);
    int y0 = static_cast<int>(y);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    double dx = x - x0;
    double dy = y - y0;
    double w00 = (1 - dx) * (1 - dy);
    double w01 = (1 - dx) * dy;
    double w10 = dx * (1 - dy);
    double w11 = dx * dy;

    std::vector<int> interpolatedValue(3, 0);
    interpolatedValue[0] = w00 * image[x0][y0][0] + w01 * image[x0][y1][0] + w10 * image[x1][y0][0] + w11 * image[x1][y1][0];
    interpolatedValue[1] = w00 * image[x0][y0][1] + w01 * image[x0][y1][1] + w10 * image[x1][y0][1] + w11 * image[x1][y1][1];
    interpolatedValue[2] = w00 * image[x0][y0][2] + w01 * image[x0][y1][2] + w10 * image[x1][y0][2] + w11 * image[x1][y1][2];
    return interpolatedValue;
}

void ImageTransform::ReverseSpatialWarping(std::string outputPath){
    RWImage rwi(height, width);

    Eigen::Matrix<double, 2, 6> affineMatrix_left, affineMatrix_right, affineMatrix_top, affineMatrix_down;
    int h = height - 1;
    int w = width - 1;

    double srcPoints_x[6] = {0, h / 2, h, h / 4, h * 3 / 4, h / 2};
    double srcPoints_y[6] = {0, 0, 0, w / 4, w / 4, w / 2};
    double dstPoints_x[6] = {0, h / 2, h, h / 4, h * 3 / 4, h / 2};
    double dstPoints_y[6] = {0, 64, 0, w / 4, w / 4, w / 2};
    affineMatrix_left = getAffineMatrix(srcPoints_x, srcPoints_y, dstPoints_x, dstPoints_y);

    srcPoints_x[0] = h;
    srcPoints_y[0] = 0;
    srcPoints_x[1] = h;
    srcPoints_y[1] = w / 2;
    srcPoints_x[2] = h;
    srcPoints_y[2] = w;
    srcPoints_x[3] = h * 3 / 4;
    srcPoints_y[3] = w / 4;
    srcPoints_x[4] = h * 3 / 4;
    srcPoints_y[4] = w * 3 / 4;
    srcPoints_x[5] = h / 2;
    srcPoints_y[5] = w / 2;
    dstPoints_x[0] = h;
    dstPoints_y[0] = 0;
    dstPoints_x[1] = h - 64;
    dstPoints_y[1] = w / 2;
    dstPoints_x[2] = h;
    dstPoints_y[2] = w;
    dstPoints_x[3] = h * 3 / 4;
    dstPoints_y[3] = w / 4;
    dstPoints_x[4] = h * 3 / 4;
    dstPoints_y[4] = w * 3 / 4;
    dstPoints_x[5] = h / 2;
    dstPoints_y[5] = w / 2;
    affineMatrix_down = getAffineMatrix(srcPoints_x, srcPoints_y, dstPoints_x, dstPoints_y);

    srcPoints_x[0] = 0;
    srcPoints_y[0] = w;
    srcPoints_x[1] = h / 2;
    srcPoints_y[1] = w;
    srcPoints_x[2] = h;
    srcPoints_y[2] = w;
    srcPoints_x[3] = h / 4;
    srcPoints_y[3] = w * 3 / 4;
    srcPoints_x[4] = h * 3 / 4;
    srcPoints_y[4] = w * 3 / 4;
    srcPoints_x[5] = h / 2;
    srcPoints_y[5] = w / 2;
    dstPoints_x[0] = 0;
    dstPoints_y[0] = w;
    dstPoints_x[1] = h / 2;
    dstPoints_y[1] = w - 64;
    dstPoints_x[2] = h;
    dstPoints_y[2] = w;
    dstPoints_x[3] = h / 4;
    dstPoints_y[3] = w * 3 / 4;
    dstPoints_x[4] = h * 3 / 4;
    dstPoints_y[4] = w * 3 / 4;
    dstPoints_x[5] = h / 2;
    dstPoints_y[5] = w / 2;
    affineMatrix_right = getAffineMatrix(srcPoints_x, srcPoints_y, dstPoints_x, dstPoints_y);

    srcPoints_x[0] = 0;
    srcPoints_y[0] = 0;
    srcPoints_x[1] = 0;
    srcPoints_y[1] = w / 2;
    srcPoints_x[2] = 0;
    srcPoints_y[2] = w;
    srcPoints_x[3] = h / 4;
    srcPoints_y[3] = w / 4;
    srcPoints_x[4] = h / 4;
    srcPoints_y[4] = w * 3 / 4;
    srcPoints_x[5] = h / 2;
    srcPoints_y[5] = w / 2;
    dstPoints_x[0] = 0;
    dstPoints_y[0] = 0;
    dstPoints_x[1] = 64;
    dstPoints_y[1] = w / 2;
    dstPoints_x[2] = 0;
    dstPoints_y[2] = w;
    dstPoints_x[3] = h / 4;
    dstPoints_y[3] = w / 4;
    dstPoints_x[4] = h / 4;
    dstPoints_y[4] = w * 3 / 4;
    dstPoints_x[5] = h / 2;
    dstPoints_y[5] = w / 2;
    affineMatrix_top = getAffineMatrix(srcPoints_x, srcPoints_y, dstPoints_x, dstPoints_y);


    std::vector<std::vector<std::vector<unsigned char> > > resultImage(height, std::vector<std::vector<unsigned char> >(width, std::vector<unsigned char>(3, 0)));
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(i > j && j < (h - i)){
                Eigen::Matrix<double, 6, 1> op;
                op.row(0) << 1;
                op.row(1) << i;
                op.row(2) << j;
                op.row(3) << (i * i);
                op.row(4) << (i * j);
                op.row(5) << (j * j);
                Eigen::Matrix<double, 2, 1> rp = affineMatrix_left * op;
                double x = rp(0,0);
                double y = rp(1,0);
                std::vector<int> mappingColor = bilinearInterpolation(x,y);
                resultImage[i][j][0] = mappingColor[0];
                resultImage[i][j][1] = mappingColor[1];
                resultImage[i][j][2] = mappingColor[2];
            }else if(i > j && j > (h - i)){
                Eigen::Matrix<double, 6, 1> op;
                op.row(0) << 1;
                op.row(1) << i;
                op.row(2) << j;
                op.row(3) << (i * i);
                op.row(4) << (i * j);
                op.row(5) << (j * j);
                Eigen::Matrix<double, 2, 1> rp = affineMatrix_down * op;
                double x = rp(0,0);
                double y = rp(1,0);
                std::vector<int> mappingColor = bilinearInterpolation(x,y);
                resultImage[i][j][0] = mappingColor[0];
                resultImage[i][j][1] = mappingColor[1];
                resultImage[i][j][2] = mappingColor[2];
            }else if(i < j && j > (h - i)){
                Eigen::Matrix<double, 6, 1> op;
                op.row(0) << 1;
                op.row(1) << i;
                op.row(2) << j;
                op.row(3) << (i * i);
                op.row(4) << (i * j);
                op.row(5) << (j * j);
                Eigen::Matrix<double, 2, 1> rp = affineMatrix_right * op;
                double x = rp(0,0);
                double y = rp(1,0);
                std::vector<int> mappingColor = bilinearInterpolation(x,y);
                resultImage[i][j][0] = mappingColor[0];
                resultImage[i][j][1] = mappingColor[1];
                resultImage[i][j][2] = mappingColor[2];
            }else if(i < j && j < (h - i)){
                Eigen::Matrix<double, 6, 1> op;
                op.row(0) << 1;
                op.row(1) << i;
                op.row(2) << j;
                op.row(3) << (i * i);
                op.row(4) << (i * j);
                op.row(5) << (j * j);
                Eigen::Matrix<double, 2, 1> rp = affineMatrix_top * op;
                double x = rp(0,0);
                double y = rp(1,0);
                std::vector<int> mappingColor = bilinearInterpolation(x,y);
                resultImage[i][j][0] = mappingColor[0];
                resultImage[i][j][1] = mappingColor[1];
                resultImage[i][j][2] = mappingColor[2];
            }else if(i == j || j == (h - i)){
                resultImage[i][j][0] = image[i][j][0];
                resultImage[i][j][1] = image[i][j][1];
                resultImage[i][j][2] = image[i][j][2];
            }
        }
    }
    image = resultImage;
    rwi.writeRawImage(outputPath, resultImage, 3);
     
}
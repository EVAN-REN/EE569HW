#include "HomographicTrans.h"
#include "RWImage.h"
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

using namespace cv;
using namespace cv::xfeatures2d;

HomographicTrans::HomographicTrans(std::string path1, std::string path2, std::string path3, int h, int w, int b):height(h), width(w), BytesPerPixel(b){
    RWImage rwi(h, w);
    image_left = rwi.readRawImage(path1, b);
    image_middle = rwi.readRawImage(path2, b);
    image_right = rwi.readRawImage(path3, b);
}


void HomographicTrans::SURF_Method(std::string outputPath){
    RWImage rwi(height, width);
    
    Mat left_toy = rwi.transformToMat(image_left);
    Mat middle_toy = rwi.transformToMat(image_middle);
    Mat right_toy = rwi.transformToMat(image_right);
    
    // Define the SURF detector
    Ptr<Feature2D> surf = SURF::create();

    // Detects and calculates key points and descriptors for the left image
    std::vector<KeyPoint> keypoints1;
    Mat descriptors1;
    surf->detectAndCompute(left_toy, noArray(), keypoints1, descriptors1);

    // The keypoints and descriptors of the middle image are detected and computed
    std::vector<KeyPoint> keypoints2;
    Mat descriptors2;
    surf->detectAndCompute(middle_toy, noArray(), keypoints2, descriptors2);

    // The keypoints and descriptors of the right image are detected and computed
    std::vector<KeyPoint> keypoints3;
    Mat descriptors3;
    surf->detectAndCompute(right_toy, noArray(), keypoints3, descriptors3);

    // Matching keypoints
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
    std::vector<std::vector<DMatch>> knn_matches1;
    std::vector<std::vector<DMatch>> knn_matches2;
    matcher->knnMatch(descriptors1, descriptors2, knn_matches1, 2);
    matcher->knnMatch(descriptors3, descriptors2, knn_matches2, 2);

    // Selecting the best match
    std::vector<DMatch> good_matches1;
    for (size_t i = 0; i < knn_matches1.size(); i++) {
        if (knn_matches1[i][0].distance < 0.3 * knn_matches1[i][1].distance) {
            good_matches1.push_back(knn_matches1[i][0]);
        }
    }

    std::vector<DMatch> good_matches2;
    for (size_t i = 0; i < knn_matches2.size(); i++) {
        if (knn_matches2[i][0].distance < 0.3 * knn_matches2[i][1].distance) {
            good_matches2.push_back(knn_matches2[i][0]);
        }
    }
    std::cout << good_matches1.size() << std::endl;
    std::cout << good_matches2.size() << std::endl;

    Mat img_matches;
    drawMatches(left_toy, keypoints1, middle_toy, keypoints2, good_matches1, img_matches);
    imshow("left and middle",img_matches);
    waitKey(0);

    Mat img_matches2;
    drawMatches(right_toy, keypoints3, middle_toy, keypoints2, good_matches2, img_matches2);
    imshow("right and middle",img_matches2);
    waitKey(0);

    int lmMatchNum = 32;
    int rmMatchNum = 32;

    std::vector<Point2f> leftPoints(lmMatchNum);
    std::vector<Point2f> middle1Points(lmMatchNum);
    std::vector<Point2f> rightPoints(rmMatchNum);
    std::vector<Point2f> middle2Points(rmMatchNum);


    // Sets the position of pixel of three graphs in the merged graph
    for (int i = 0; i < lmMatchNum; i++) {
        // Get the index of the current match
        int queryIdx1 = good_matches1[i].queryIdx;
        int trainIdx1 = good_matches1[i].trainIdx;

        // Get the corresponding keypoint coordinates
        leftPoints[i] = keypoints1[queryIdx1].pt;
        leftPoints[i].x += width / 2;
        leftPoints[i].y += height / 2;
        middle1Points[i] = keypoints2[trainIdx1].pt;
        middle1Points[i].x += width;
        middle1Points[i].y += height / 2;
    }

    for (int i = 0; i < rmMatchNum; i++) {
        int queryIdx2 = good_matches2[i].queryIdx;
        int trainIdx2 = good_matches2[i].trainIdx;

        rightPoints[i] = keypoints3[queryIdx2].pt;
        rightPoints[i].x += width * 3 / 2;
        rightPoints[i].y += height / 2;
        middle2Points[i] = keypoints2[trainIdx2].pt;
        middle2Points[i].x += width;
        middle2Points[i].y += height / 2;
    }

    // calculate two sets of transformation matrices according to the matching points
    cv::Mat H1 = cv::findHomography(leftPoints, middle1Points, cv::RANSAC);
    cv::Mat H2 = cv::findHomography(rightPoints, middle2Points, cv::RANSAC);

    // copy three images to new images which position is in merge graph  
    cv::Mat new_left_toy = cv::Mat::zeros(2 * height, 3 * width, CV_8UC3); 
    cv::Mat roi1 = new_left_toy(cv::Rect(width / 2, height / 2, width, height)); 
    left_toy.copyTo(roi1);

    cv::Mat new_middle_toy = cv::Mat::zeros(2 * height, 3 * width, CV_8UC3); 
    cv::Mat roi2 = new_middle_toy(cv::Rect(width, height / 2, width, height)); 
    middle_toy.copyTo(roi2);

    cv::Mat new_right_toy = cv::Mat::zeros(2 * height, 3 * width, CV_8UC3); 
    cv::Mat roi3 = new_right_toy(cv::Rect(width * 3 / 2, height / 2, width, height)); 
    right_toy.copyTo(roi3);

    // transform image
    cv::Mat resMat1,resMat2;
    warpPerspective(new_left_toy, resMat1, H1, Size(3 * width, 2 * height));
    warpPerspective(new_right_toy, resMat2, H2, Size(3 * width, 2 * height));

    // combine three image
    std::vector<std::vector<std::vector<unsigned char> > > result(height * 2, std::vector<std::vector<unsigned char> >(width * 3, std::vector<unsigned char>(3, 0)));

    for(int i = 0; i < 2 * height; i++){
        for(int j = 0; j < 3 * width; j++){
            if(new_middle_toy.at<cv::Vec3b>(i, j) != cv::Vec3b(0,0,0)){
                result[i][j][0] = new_middle_toy.at<cv::Vec3b>(i, j)[2];
                result[i][j][1] = new_middle_toy.at<cv::Vec3b>(i, j)[1];
                result[i][j][2] = new_middle_toy.at<cv::Vec3b>(i, j)[0];
            }else if(resMat1.at<cv::Vec3b>(i, j) != cv::Vec3b(0,0,0)){
                result[i][j][0] = resMat1.at<cv::Vec3b>(i, j)[2];
                result[i][j][1] = resMat1.at<cv::Vec3b>(i, j)[1];
                result[i][j][2] = resMat1.at<cv::Vec3b>(i, j)[0];
            }else if(resMat2.at<cv::Vec3b>(i, j) != cv::Vec3b(0,0,0)){
                result[i][j][0] = resMat2.at<cv::Vec3b>(i, j)[2];
                result[i][j][1] = resMat2.at<cv::Vec3b>(i, j)[1];
                result[i][j][2] = resMat2.at<cv::Vec3b>(i, j)[0];
            }
        }
    }

    rwi.writeRawImage(outputPath, result, 3);
}


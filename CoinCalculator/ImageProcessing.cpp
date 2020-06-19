//
//  ImageProcessing.cpp
//
//  Created by Junyoung Yang on 06/18/20.
//  Copyright (c) 2020 Junyoung Yang. All rights reserved.
//
#include "ImageProcessing.h"

using namespace cv;
using namespace std;

ImageProcessor::ImageProcessor()
{

}

ImageProcessor::ImageProcessor(Mat inputImg)
{
    this->inputImg = inputImg;
}

vector<Vec3f> ImageProcessor::findCircles()
{
    vector<Vec3f> coins;

    // STEP #1 : 그레이 스케일 영상으로 변환한다.
    cvtColor(inputImg, grayImg, COLOR_BGR2GRAY);

    // STEP #2 : 그레이 스케일 영상을 이진화 한다. (Otsu Thresh)
    threshold(grayImg, binaryImg, 50, 255, THRESH_BINARY | THRESH_OTSU);

    // STEP #3 : 가우시안 필터를 적용해 잡음을 제거한다.
    GaussianBlur(binaryImg, gaussian, Size(3, 3), 3, 3);

    // 7x7 사각형 구조요소를 생성한다. 원점은 (3,3)이다.
    Mat morphMask = getStructuringElement(MORPH_RECT, Size(7, 7), Point(3, 3));
    
    // STEP #4 : Closing 연산을 통해 움푹 파인 영역을 채워준다.  
    Mat closing;
    morphologyEx(gaussian, closing, MORPH_CLOSE, morphMask);

    // STEP #5 : Canny Edge Detector를 사용해 Edge를 찾는다. 
    Canny(closing, cannyEdge, 100, 175, 3);
   
    // STEP #6 : Hough Circle Transform으로 영상 내의 원을 찾는다.
    // @param5 minDist : 원 사이의 최소 거리
    // @param6 param1 : Canny Edge Detector에서 사용할 높은 Threshold 값
    // @param7 param2 : Accumulator의 Threshold 값, 너무 작으면 거짓 원이 검출된다.
    // @param8 minRadius : 검출할 원의 최소 반지름 
    // @param9 max Radius : 검출할 원의 최대 반지름
    HoughCircles(cannyEdge, coins, HOUGH_GRADIENT, 1, 30, 150, 20, 30, 65);

    if (coins.size() > 25)
        coins.clear();

    return coins;
}

void ImageProcessor::makeMasks(Mat& maskImg, vector<Vec3f> coins)
{
    for (size_t i = 0; i < coins.size(); i++)
    {
        Point center(cvRound(coins[i][0]), cvRound(coins[i][1]));
        int radius = cvRound(coins[i][2]);

        // 속이 꽉찬 흰색 원(Mask)을 그린다.
        circle(maskImg, center, radius, Scalar(255, 255, 255), -1);
    }
}

void ImageProcessor::printUSDResult(Mat& outputImg, float sumAmount)
{
    // 계산된 금액을 영상에 추가하기 위해 문자열로 파싱한다.
    char msg[50] = "";
    sprintf(msg, "Total Amount = %.2f dollars", sumAmount);

    // 영상에 문자열을 띄운다.
    putText(outputImg, msg, Point(20, 50), FONT_HERSHEY_COMPLEX, 0.8, Scalar(255, 255, 255), 3);
}

void ImageProcessor::printKRWResult(Mat& outputImg, float sumAmount)
{
    // 계산된 금액을 영상에 추가하기 위해 문자열로 파싱한다.
    char msg[50] = "";
    sprintf(msg, "Total Amount = %d won", (int)sumAmount);

    // 영상에 문자열을 띄운다.
    putText(outputImg, msg, Point(20, 50), FONT_HERSHEY_COMPLEX, 0.8, Scalar(255, 255, 255), 3);
}
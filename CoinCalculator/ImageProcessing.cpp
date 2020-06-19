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

    // STEP #1 : �׷��� ������ �������� ��ȯ�Ѵ�.
    cvtColor(inputImg, grayImg, COLOR_BGR2GRAY);

    // STEP #2 : �׷��� ������ ������ ����ȭ �Ѵ�. (Otsu Thresh)
    threshold(grayImg, binaryImg, 50, 255, THRESH_BINARY | THRESH_OTSU);

    // STEP #3 : ����þ� ���͸� ������ ������ �����Ѵ�.
    GaussianBlur(binaryImg, gaussian, Size(3, 3), 3, 3);

    // 7x7 �簢�� ������Ҹ� �����Ѵ�. ������ (3,3)�̴�.
    Mat morphMask = getStructuringElement(MORPH_RECT, Size(7, 7), Point(3, 3));
    
    // STEP #4 : Closing ������ ���� ��ǫ ���� ������ ä���ش�.  
    Mat closing;
    morphologyEx(gaussian, closing, MORPH_CLOSE, morphMask);

    // STEP #5 : Canny Edge Detector�� ����� Edge�� ã�´�. 
    Canny(closing, cannyEdge, 100, 175, 3);
   
    // STEP #6 : Hough Circle Transform���� ���� ���� ���� ã�´�.
    // @param5 minDist : �� ������ �ּ� �Ÿ�
    // @param6 param1 : Canny Edge Detector���� ����� ���� Threshold ��
    // @param7 param2 : Accumulator�� Threshold ��, �ʹ� ������ ���� ���� ����ȴ�.
    // @param8 minRadius : ������ ���� �ּ� ������ 
    // @param9 max Radius : ������ ���� �ִ� ������
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

        // ���� ���� ��� ��(Mask)�� �׸���.
        circle(maskImg, center, radius, Scalar(255, 255, 255), -1);
    }
}

void ImageProcessor::printUSDResult(Mat& outputImg, float sumAmount)
{
    // ���� �ݾ��� ���� �߰��ϱ� ���� ���ڿ��� �Ľ��Ѵ�.
    char msg[50] = "";
    sprintf(msg, "Total Amount = %.2f dollars", sumAmount);

    // ���� ���ڿ��� ����.
    putText(outputImg, msg, Point(20, 50), FONT_HERSHEY_COMPLEX, 0.8, Scalar(255, 255, 255), 3);
}

void ImageProcessor::printKRWResult(Mat& outputImg, float sumAmount)
{
    // ���� �ݾ��� ���� �߰��ϱ� ���� ���ڿ��� �Ľ��Ѵ�.
    char msg[50] = "";
    sprintf(msg, "Total Amount = %d won", (int)sumAmount);

    // ���� ���ڿ��� ����.
    putText(outputImg, msg, Point(20, 50), FONT_HERSHEY_COMPLEX, 0.8, Scalar(255, 255, 255), 3);
}
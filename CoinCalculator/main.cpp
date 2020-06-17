//
//  main.cpp
//
//  Created by Junyoung Yang on 06/18/20.
//  Copyright (c) 2020 Junyoung Yang. All rights reserved.
//
#include "functions.h"

int main()
{
    Mat inputImg, grayImg, binaryImg;
    Mat gaussian, cannyEdge;

    // mode == 1 : Swedish Krona
    // mode == 2 : Korean Won
    int mode;

    cout << "Available Options..." << endl;
    cout << "Option 1 : Swedish Krona" << endl;
    cout << "Option 2 : Korean Won" << endl;
    cout << "Enter Option >> ";
    cin >> mode;
    
    //���� ĸ�� �ʱ�ȭ
    VideoCapture camera(0);
    if (!camera.isOpened())
    {
        cerr << "���� - ī�޶� �� �� �����ϴ�.\n";
        return -1;
    }

    while (1)
    {
        float sumAmount = 0;

        // ī�޶�κ��� ĸ���� ������ frame�� �����մϴ�.
        camera.read(inputImg);
        if (inputImg.empty())
        {
            cerr << "�� ������ ĸ�ĵǾ����ϴ�.\n";
            break;
        }

        // �׷��� ������ �������� ��ȯ�Ѵ�.
        cvtColor(inputImg, grayImg, COLOR_BGR2GRAY);

        // �׷��� ������ ������ ����ȭ �Ѵ�.
        threshold(grayImg, binaryImg, 150, 255, THRESH_BINARY | THRESH_OTSU);

        // Canny Edge�� ã�´�.
        GaussianBlur(binaryImg, gaussian, Size(3, 3), 3, 3);

        Mat morphMask = getStructuringElement(MORPH_RECT, Size(7, 7), Point(3, 3));
        Mat closing;
        morphologyEx(gaussian, closing, MORPH_CLOSE, morphMask);

        imshow("closing", closing);

        Canny(closing, cannyEdge, 100, 175, 3);

        imshow("Canny Edge", cannyEdge);

        // Closing ������ ���� ������ ������ �����Ѵ�.
        //Mat morphMask = getStructuringElement(MORPH_RECT, Size(3, 3), Point(1, 1));
        //Mat closed;
        //morphologyEx(cannyEdge, closed, MORPH_CLOSE, morphMask);
        //Mat dilation;
        //dilate(cannyEdge, dilation, morphMask, Point(-1, -1), 1, 0);
   
        vector<Vec3f> coins, silverCoins, copperCoins;
        // Hough Circle Transform���� ���� ���� ���� ã�´�.
        // @param minDist : �� ������ �ּ� �Ÿ�
        // @param param1 : Canny Edge Detector���� ����� ���� Threshold ��
        // @param param2 : Accumulator�� Threshold ��, �ʹ� ������ ���� ���� ����ȴ�
        // @param minRadius, max Radius : ������ ���� �ּ� �ִ� ������
        HoughCircles(cannyEdge, coins, HOUGH_GRADIENT, 1, 30, 150, 20, 30, 65);

        Mat coinImg = inputImg.clone();
        Mat maskImg = inputImg.clone();
        Mat bitWise = inputImg - (~maskImg);
        // Mat bitWise = inputImg + (~maskImg);

        imshow("Bitwise", bitWise);

        Mat hsvImg;    // ������ ���� ������ Ȱ���ϱ� ���� HSV ���� �������� ��ȯ�Ѵ�.
        cvtColor(bitWise, hsvImg, COLOR_BGR2HSV);
        
        imshow("HSV", hsvImg);

        // Hue ���� �̿��� ���� ����, ������ �������� �����Ѵ�.
        classificationByColor(hsvImg, coins, silverCoins, copperCoins);

        // �νĵ� �������� �׼� ���� ����Ѵ�.
        // calcSumAmount(coinImg, maskImg, coins, sumAmount);
        improvedCalcSumAmount(coinImg, maskImg, silverCoins, copperCoins, sumAmount);

        char msg[50] = "";
        sprintf(msg, "Total Amount = %.2f dollars", sumAmount);

        putText(coinImg, msg, Point(20, 50), FONT_HERSHEY_COMPLEX, 0.8, Scalar(255, 255, 255), 3);

        // ������ ȭ�鿡 �����ݴϴ�. 
        imshow("Result", coinImg);

        // ESC Ű�� �Է��ϸ� ������ ����˴ϴ�. 
        if (waitKey(25) >= 0)
            break;
    }

    return 0;
}
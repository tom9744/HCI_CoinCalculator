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
    
    //비디오 캡쳐 초기화
    VideoCapture camera(0);
    if (!camera.isOpened())
    {
        cerr << "에러 - 카메라를 열 수 없습니다.\n";
        return -1;
    }

    while (1)
    {
        float sumAmount = 0;

        // 카메라로부터 캡쳐한 영상을 frame에 저장합니다.
        camera.read(inputImg);
        if (inputImg.empty())
        {
            cerr << "빈 영상이 캡쳐되었습니다.\n";
            break;
        }

        // 그레이 스케일 영상으로 변환한다.
        cvtColor(inputImg, grayImg, COLOR_BGR2GRAY);

        // 그레이 스케일 영상을 이진화 한다.
        threshold(grayImg, binaryImg, 150, 255, THRESH_BINARY | THRESH_OTSU);

        // Canny Edge를 찾는다.
        GaussianBlur(binaryImg, gaussian, Size(3, 3), 3, 3);

        Mat morphMask = getStructuringElement(MORPH_RECT, Size(7, 7), Point(3, 3));
        Mat closing;
        morphologyEx(gaussian, closing, MORPH_CLOSE, morphMask);

        imshow("closing", closing);

        Canny(closing, cannyEdge, 100, 175, 3);

        imshow("Canny Edge", cannyEdge);

        // Closing 연산을 통해 끊어진 엣지를 연결한다.
        //Mat morphMask = getStructuringElement(MORPH_RECT, Size(3, 3), Point(1, 1));
        //Mat closed;
        //morphologyEx(cannyEdge, closed, MORPH_CLOSE, morphMask);
        //Mat dilation;
        //dilate(cannyEdge, dilation, morphMask, Point(-1, -1), 1, 0);
   
        vector<Vec3f> coins, silverCoins, copperCoins;
        // Hough Circle Transform으로 영상 내의 원을 찾는다.
        // @param minDist : 원 사이의 최소 거리
        // @param param1 : Canny Edge Detector에서 사용할 높은 Threshold 값
        // @param param2 : Accumulator의 Threshold 값, 너무 작으면 거짓 원이 검출된다
        // @param minRadius, max Radius : 검출할 원의 최소 최대 반지름
        HoughCircles(cannyEdge, coins, HOUGH_GRADIENT, 1, 30, 150, 20, 30, 65);

        Mat coinImg = inputImg.clone();
        Mat maskImg = inputImg.clone();
        Mat bitWise = inputImg - (~maskImg);
        // Mat bitWise = inputImg + (~maskImg);

        imshow("Bitwise", bitWise);

        Mat hsvImg;    // 동전의 색상 정보를 활용하기 위해 HSV 색상 공간으로 전환한다.
        cvtColor(bitWise, hsvImg, COLOR_BGR2HSV);
        
        imshow("HSV", hsvImg);

        // Hue 값을 이용해 은색 동전, 구리색 동전으로 구분한다.
        classificationByColor(hsvImg, coins, silverCoins, copperCoins);

        // 인식된 동전들의 액수 합을 계산한다.
        // calcSumAmount(coinImg, maskImg, coins, sumAmount);
        improvedCalcSumAmount(coinImg, maskImg, silverCoins, copperCoins, sumAmount);

        char msg[50] = "";
        sprintf(msg, "Total Amount = %.2f dollars", sumAmount);

        putText(coinImg, msg, Point(20, 50), FONT_HERSHEY_COMPLEX, 0.8, Scalar(255, 255, 255), 3);

        // 영상을 화면에 보여줍니다. 
        imshow("Result", coinImg);

        // ESC 키를 입력하면 루프가 종료됩니다. 
        if (waitKey(25) >= 0)
            break;
    }

    return 0;
}
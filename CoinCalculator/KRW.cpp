//
//  KRW.cpp
//
//  Created by Junyoung Yang on 06/18/20.
//  Copyright (c) 2020 Junyoung Yang. All rights reserved.
//
#include "KRW.h"

KRW::KRW()
{

}

int KRW::getRandom(int radius)
{
    // Mersenne Twister를 사용한 임의의 수 생성
    random_device rand;
    mt19937 mersenne(rand());

    int range = (radius / 2);

    return uniform_int_distribution<>(-range, range)(mersenne);
}

// Hue 값을 사용하지 않고 오직 반지름 만으로 판별하는 메서드
// 보고서 작성 시, 정확도에 대한 Benchmark로 사용할 예정.
void KRW::calcSumAmount(Mat& coinImg, vector<Vec3f> coins, float& sumAmount)
{
    for (size_t i = 0; i < coins.size(); i++)
    {
        Point center(cvRound(coins[i][0]), cvRound(coins[i][1]));
        int radius = cvRound(coins[i][2]);

        /*************************************/
        /*   제한 요소에 따른 각 동전의 값   */
        /*     500원 동전 | Radius 55~58     */
        /*     100원 동전 | Radius 49~53     */
        /*     50원  동전 | Radius 45~48     */
        /*     10원  동전 | Radius 37~40     */
        /* (구)10원  동전 | Radius 47~51     */
        /*************************************/

        if (radius >= 55)
        {
            // circle(coinImg, center, radius, Scalar(0, 225, 255), 2, 8, 0);
            sumAmount += 500;
        }
        else if (radius >= 49 && radius <= 54)
        {
            // circle(coinImg, center, radius, Scalar(255, 222, 255), 2, 8, 0);
            sumAmount += 100;
        }
        else if (radius >= 44 && radius <= 48)
        {
            // circle(coinImg, center, radius, Scalar(255, 0, 255), 2, 8, 0);
            sumAmount += 50;
        }
        else if (radius >= 36 && radius <= 41)
        {
            // circle(coinImg, center, radius, Scalar(0, 0, 255), 2, 8, 0);
            sumAmount += 10;
        }
        else if (radius > 45 && radius <= 52)
        {
            // circle(coinImg, center, radius, Scalar(255, 0, 0), 2, 8, 0);
            sumAmount += 10;
        }
    }
}

void KRW::improvedCalcSumAmount(Mat& coinImg, vector<Vec3f> silverCoins, vector<Vec3f> copperCoins, float& sumAmount)
{
    /**********************************************/
    /*        제한 요소에 따른 각 동전의 값       */
    /*     500원 동전 | Radius 55~58 | Silver     */
    /*     100원 동전 | Radius 49~53 | Silver     */
    /*     50원  동전 | Radius 45~48 | Silver     */
    /*     10원  동전 | Radius 37~40 | Copper     */
    /* (구)10원  동전 | Radius 47~51 | Copper     */
    /**********************************************/

    for (size_t i = 0; i < silverCoins.size(); i++)
    {
        Point center(cvRound(silverCoins[i][0]), cvRound(silverCoins[i][1]));
        int radius = cvRound(silverCoins[i][2]);

        // cout << radius << endl;

        if (radius >= 55)
        {
            // circle(coinImg, center, radius, Scalar(0, 225, 255), 2, 8, 0);
            sumAmount += 500;
        }
        else if (radius >= 49 && radius <= 53)
        {
            // circle(coinImg, center, radius, Scalar(255, 222, 255), 2, 8, 0);
            sumAmount += 100;
        }
        else if (radius >= 44 && radius <= 48)
        {
            // circle(coinImg, center, radius, Scalar(255, 0, 255), 2, 8, 0);
            sumAmount += 50;
        }
    }

    for (size_t i = 0; i < copperCoins.size(); i++)
    {
        Point center(cvRound(copperCoins[i][0]), cvRound(copperCoins[i][1]));
        int radius = cvRound(copperCoins[i][2]);

        // cout << radius << endl;

        if (radius >= 36 && radius <= 41)
        {
            // circle(coinImg, center, radius, Scalar(0, 0, 255), 2, 8, 0);
            sumAmount += 10;
        }
        else if(radius >= 45 && radius <= 52)
        {
            // circle(coinImg, center, radius, Scalar(255, 0, 0), 2, 8, 0);
            sumAmount += 10;
        }
    }
}

void KRW::classificationByColor(Mat hsvImg, vector<Vec3f> coins, vector<Vec3f>& silverCoins, vector<Vec3f>& copperCoins)
{
    for (size_t i = 0; i < coins.size(); i++)
    {
        // 원에 대한 정보(x, y, r)를 구한다.
        int center_x = cvRound(coins[i][0]);
        int center_y = cvRound(coins[i][1]);
        int radius = cvRound(coins[i][2]);

        // ROI(관심 영역) 지정을 위한 Rect 객체 생성에 필요한 인자
        int startPointX = center_x - radius;
        int startPointY = center_y - radius;

        // ROI(관심 역역)을 추출한다.
        Rect rectangle(startPointX, startPointY, radius * 2, radius * 2);
        Rect bounds(0, 0, hsvImg.cols, hsvImg.rows);

        Mat ROI;
        if (startPointX > 0 && startPointY > 0)
        {
            // Bound를 사용한 예외처리와 함께, ROI(관심 영역)에 대한 Mat 객체 생성
            ROI = hsvImg(rectangle & bounds);
        }

        // imshow("ROI", ROI); 디버깅...

        int roiCenterX = ROI.cols / 2;
        int roiCenterY = ROI.rows / 2;

        int accumulatedHueValue = 0;
        int averageHueValue = 0;

        try
        {
            // 동전 내부의 20개 임의 위치에서의 Hue값의 평균을 계산한다.
            // 임의의 위치가 고르게 분포할 수 있도록 Mersenne Twister 난수 발생기를 사용한다.
            for (size_t i = 0; i < 20; i++)
            {
                int randX = roiCenterX + getRandom(radius);
                int randY = roiCenterY + getRandom(radius);

                if (randX < 0 || randY < 0)
                    continue;

                // 임의의 좌표의 픽셀의 Hue 값을 누적한다.
                accumulatedHueValue += (int)ROI.at<Vec3b>(randY, randX)[0];
            }
            // 평균값을 계산한다.
            averageHueValue = accumulatedHueValue / 20;

            // cout << averageHueValue << endl;
        }
        // Out-of-Boundary Exception에 대한 처리
        catch (Exception err)
        {
            // cout << err.what();
            continue;
        }

        // 20개 임의 픽셀의 평균 Hue 값이 35보다 크면 구리 동전이 아닌 것으로 판단
        if (averageHueValue > 35)
            silverCoins.push_back(coins[i]);
        else
            copperCoins.push_back(coins[i]);
    }

    // For Debugging
    // cout << "Copper 동전 Class에 속하는 동전 개수 : " << copperCoins.size() << endl;
    // cout << "Silver 동전 Class에 속하는 동전 개수 : " << silverCoins.size() << endl;
}
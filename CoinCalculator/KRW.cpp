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
    // Mersenne Twister�� ����� ������ �� ����
    random_device rand;
    mt19937 mersenne(rand());

    int range = (radius / 2);

    return uniform_int_distribution<>(-range, range)(mersenne);
}

// Hue ���� ������� �ʰ� ���� ������ ������ �Ǻ��ϴ� �޼���
// ���� �ۼ� ��, ��Ȯ���� ���� Benchmark�� ����� ����.
void KRW::calcSumAmount(Mat& coinImg, Mat& maskImg, vector<Vec3f> coins, float& sumAmount)
{
    for (size_t i = 0; i < coins.size(); i++)
    {
        Point center(cvRound(coins[i][0]), cvRound(coins[i][1]));
        int radius = cvRound(coins[i][2]);

        /*************************************/
        /*   ���� ��ҿ� ���� �� ������ ��   */
        /*     500�� ���� | Radius 55~58     */
        /*     100�� ���� | Radius 49~53     */
        /*     50��  ���� | Radius 45~48     */
        /*     10��  ���� | Radius 37~40     */
        /* (��)10��  ���� | Radius 47~51     */
        /*************************************/

        if (radius >= 55)
        {
            // circle(coinImg, center, radius, Scalar(0, 0, 255), 2, 8, 0);
            sumAmount += 1;
        }
        else if (radius >= 50 && radius <= 53)
        {
            // circle(coinImg, center, radius, Scalar(0, 225, 255), 2, 8, 0);
            sumAmount += 0.25;
        }
        else if (radius >= 37 && radius <= 39)
        {
            // circle(coinImg, center, radius, Scalar(255, 222, 255), 2, 8, 0);
            sumAmount += 0.10;
        }
        else if (radius >= 45 && radius <= 47)
        {
            // circle(coinImg, center, radius, Scalar(255, 0, 255), 2, 8, 0);
            sumAmount += 0.05;
        }
        else if (radius > 39 && radius <= 41)
        {
            // circle(coinImg, center, radius, Scalar(255, 0, 0), 2, 8, 0);
            sumAmount += 0.01;
        }
    }
}

void KRW::improvedCalcSumAmount(Mat& coinImg, Mat& maskImg, vector<Vec3f> silverCoins, vector<Vec3f> copperCoins, float& sumAmount)
{
    /**********************************************/
    /*        ���� ��ҿ� ���� �� ������ ��       */
    /*     500�� ���� | Radius 55~58 | Silver     */
    /*     100�� ���� | Radius 49~53 | Silver     */
    /*     50��  ���� | Radius 45~48 | Silver     */
    /*     10��  ���� | Radius 37~40 | Copper     */
    /* (��)10��  ���� | Radius 47~51 | Copper     */
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
        // ���� ���� ����(x, y, r)�� ���Ѵ�.
        int center_x = cvRound(coins[i][0]);
        int center_y = cvRound(coins[i][1]);
        int radius = cvRound(coins[i][2]);

        // ROI(���� ����) ������ ���� Rect ��ü ������ �ʿ��� ����
        int startPointX = center_x - radius;
        int startPointY = center_y - radius;

        // ROI(���� ����)�� �����Ѵ�.
        Rect rectangle(startPointX, startPointY, radius * 2, radius * 2);
        Rect bounds(0, 0, hsvImg.cols, hsvImg.rows);

        Mat ROI;
        if (startPointX > 0 && startPointY > 0)
        {
            // Bound�� ����� ����ó���� �Բ�, ROI(���� ����)�� ���� Mat ��ü ����
            ROI = hsvImg(rectangle & bounds);
        }

        // imshow("ROI", ROI); �����...

        int roiCenterX = ROI.cols / 2;
        int roiCenterY = ROI.rows / 2;

        int accumulatedHueValue = 0;
        int averageHueValue = 0;

        try
        {
            // ���� ������ 50�� ���� ��ġ������ Hue���� ����� ����Ѵ�.
            // ������ ��ġ�� ���� ������ �� �ֵ��� Mersenne Twister ���� �߻��⸦ ����Ѵ�.
            for (size_t i = 0; i < 50; i++)
            {
                int randX = roiCenterX + getRandom(radius);
                int randY = roiCenterY + getRandom(radius);

                if (randX < 0 || randY < 0)
                    continue;

                // ������ ��ǥ�� �ȼ��� Hue ���� �����Ѵ�.
                accumulatedHueValue += (int)ROI.at<Vec3b>(randY, randX)[0];
            }
            // ��հ��� ����Ѵ�.
            averageHueValue = accumulatedHueValue / 50;

            cout << averageHueValue << endl;
        }
        // Out-of-Boundary Exception�� ���� ó��
        catch (Exception err)
        {
            // cout << err.what();
            continue;
        }

        // 50�� ���� �ȼ��� ��� Hue ���� 50���� ũ�� �������� �Ǵ�
        if (averageHueValue > 35)
            silverCoins.push_back(coins[i]);
        else
            copperCoins.push_back(coins[i]);
    }

    // For Debugging
    // cout << "Copper ���� Class�� ���ϴ� ���� ���� : " << copperCoins.size() << endl;
    // cout << "Silver ���� Class�� ���ϴ� ���� ���� : " << silverCoins.size() << endl;
}
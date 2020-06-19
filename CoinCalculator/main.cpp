//
//  main.cpp
//
//  Created by Junyoung Yang on 06/18/20.
//  Copyright (c) 2020 Junyoung Yang. All rights reserved.
//
#include "ImageProcessing.h"
#include "USD.h"
#include "KRW.h"

int main()
{
    Mat inputImg;

    // mode == 1 : USD
    // mode == 2 : KRW
    int mode = 0;

    cout << "[Auto Coin Calculator]" << endl;
    cout << "Select Initial Mode..." << endl;
    cout << "Option 1 : USD" << endl;
    cout << "Option 2 : KRW" << endl;
    
    // ��� ���ÿ� ���� ����ó��
    while (1)
    {
        cout << "Enter Option >> ";
        cin >> mode;

        if (!cin)
        {
            cout << "ERROR: Forbidden character" << endl;
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
        else
        {
            if (mode == 1 || mode == 2)
            {
                if (mode == 1)
                    cout << "You've selected USD mode" << endl;
                else
                    cout << "You've selected KRW mode" << endl;

                break;
            }
            else
                cout << "ERROR: You have selected invalid mode, Try again." << endl;
        }
    }
    
    //���� ĸ�� �ʱ�ȭ
    VideoCapture camera(0);
    if (!camera.isOpened())
    {
        cerr << "ERROR: Can't open the camera.\n";
        return -1;
    }

    while (1)
    {
        float sumAmount = 0;
        vector<Vec3f> coins, silverCoins, copperCoins;

        // ī�޶�κ��� ĸ���� ������ frame�� �����մϴ�.
        camera.read(inputImg);
        if (inputImg.empty())
        {
            cerr << "ERROR: Image is empty.\n";
            break;
        }

        ImageProcessor postProc(inputImg);
        coins = postProc.findCircles();

        Mat coinImg = inputImg.clone();
        Mat maskImg = inputImg.clone();

        postProc.makeMasks(maskImg, coins);

        Mat bitWise = inputImg - (~maskImg);
        // Mat bitWise = inputImg + (~maskImg);

        // imshow("Bitwise", bitWise);

        Mat hsvImg;    // ������ ���� ������ Ȱ���ϱ� ���� HSV ���� �������� ��ȯ�Ѵ�.
        cvtColor(bitWise, hsvImg, COLOR_BGR2HSV);

        // imshow("HSV", hsvImg);

        if (mode == 1)
        {
            USD dollar;

            // Hue ���� �̿��� ���� ����, ������ �������� �����Ѵ�.
            dollar.classificationByColor(hsvImg, coins, silverCoins, copperCoins);

            // �νĵ� �������� �׼� ���� ����Ѵ�.
            // calcSumAmount(coinImg, maskImg, coins, sumAmount);
            dollar.improvedCalcSumAmount(coinImg, maskImg, silverCoins, copperCoins, sumAmount);

            postProc.printUSDResult(coinImg, sumAmount);
        }

        else
        {
            KRW won;

            // Hue ���� �̿��� ���� ����, ������ �������� �����Ѵ�.
            won.classificationByColor(hsvImg, coins, silverCoins, copperCoins);

            // �νĵ� �������� �׼� ���� ����Ѵ�.
            // calcSumAmount(coinImg, maskImg, coins, sumAmount);
            won.improvedCalcSumAmount(coinImg, maskImg, silverCoins, copperCoins, sumAmount);

            postProc.printKRWResult(coinImg, sumAmount);
        }

        // ������ ȭ�鿡 �����ݴϴ�. 
        imshow("Result", coinImg);

        // ESC Ű�� �Է��ϸ� ������ ����˴ϴ�. 
        int key = waitKey(1);

        if (key == 27)       // ESC key
            break;
        else if (key == 100) // 'D' key
            mode = 1;
        else if (key == 119) // 'W' key
            mode = 2;
    }

    cout << "Terminating the calculator..." << endl;

    return 0;
}
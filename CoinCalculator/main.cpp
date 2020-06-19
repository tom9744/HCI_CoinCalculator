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
    
    // 모드 선택에 대한 예외처리
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
    
    //비디오 캡쳐 초기화
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

        // 카메라로부터 캡쳐한 영상을 frame에 저장합니다.
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

        Mat hsvImg;    // 동전의 색상 정보를 활용하기 위해 HSV 색상 공간으로 전환한다.
        cvtColor(bitWise, hsvImg, COLOR_BGR2HSV);

        // imshow("HSV", hsvImg);

        if (mode == 1)
        {
            USD dollar;

            // Hue 값을 이용해 은색 동전, 구리색 동전으로 구분한다.
            dollar.classificationByColor(hsvImg, coins, silverCoins, copperCoins);

            // 인식된 동전들의 액수 합을 계산한다.
            // calcSumAmount(coinImg, maskImg, coins, sumAmount);
            dollar.improvedCalcSumAmount(coinImg, maskImg, silverCoins, copperCoins, sumAmount);

            postProc.printUSDResult(coinImg, sumAmount);
        }

        else
        {
            KRW won;

            // Hue 값을 이용해 은색 동전, 구리색 동전으로 구분한다.
            won.classificationByColor(hsvImg, coins, silverCoins, copperCoins);

            // 인식된 동전들의 액수 합을 계산한다.
            // calcSumAmount(coinImg, maskImg, coins, sumAmount);
            won.improvedCalcSumAmount(coinImg, maskImg, silverCoins, copperCoins, sumAmount);

            postProc.printKRWResult(coinImg, sumAmount);
        }

        // 영상을 화면에 보여줍니다. 
        imshow("Result", coinImg);

        // ESC 키를 입력하면 루프가 종료됩니다. 
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
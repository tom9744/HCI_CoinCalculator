//
//  functions.h
//
//  Created by Junyoung Yang on 06/18/20.
//  Copyright (c) 2020 Junyoung Yang. All rights reserved.
//
#define _CRT_SECURE_NO_WARNINGS

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <random>

using namespace cv;
using namespace std;

int getRandom(int radius);
void calcSumAmount(Mat& coinImg, Mat& maskImg, vector<Vec3f> coins, float& sumAmount);
void improvedCalcSumAmount(Mat& coinImg, Mat& maskImg, vector<Vec3f> silverCoins, vector<Vec3f> copperCoins, float& sumAmount);
void classificationByColor(Mat hsvImg, vector<Vec3f> coins, vector<Vec3f>& silverCoins, vector<Vec3f>& copperCoins);
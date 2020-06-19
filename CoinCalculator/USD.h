#pragma once
//
//  USD.h
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

// USD Mode와 관련된 클래스
class USD
{
private:
	// 원의 중짐점에서 상하좌우 (radius / 2) 범위의 임의 좌표를 반환하는 메서드
	int getRandom(int radius);
public:
	USD();
	// Hue 값을 사용하지 않고 오직 반지름 만으로 판별하는 메서드. 보고서 작성 시, 정확도에 대한 Benchmark로 사용할 예정.
	void calcSumAmount(Mat& coinImg, Mat& maskImg, vector<Vec3f> coins, float& sumAmount);
	// Hue 값과 동전의 반지름 두 특징점을 사용해 동전을 판별하는 메서드.
	void improvedCalcSumAmount(Mat& coinImg, Mat& maskImg, vector<Vec3f> silverCoins, vector<Vec3f> copperCoins, float& sumAmount);
	// 동전의 Hue 값을 기준으로 Copper(구리) 동전인지, Silver(은) 동전인지 구분하여 두개의 vector에 담는 메서드.
	void classificationByColor(Mat hsvImg, vector<Vec3f> coins, vector<Vec3f>& silverCoins, vector<Vec3f>& copperCoins);
};

#pragma once
//
//  KRW.h
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

class KRW
{
private:
	// ���� ���������� �����¿� (radius / 2) ������ ���� ��ǥ�� ��ȯ�ϴ� �޼���
	int getRandom(int radius);
public:
	KRW();
	// Hue ���� ������� �ʰ� ���� ������ ������ �Ǻ��ϴ� �޼���. ���� �ۼ� ��, ��Ȯ���� ���� Benchmark�� ����� ����.
	void calcSumAmount(Mat& coinImg, vector<Vec3f> coins, float& sumAmount);
	// Hue ���� ������ ������ �� Ư¡���� ����� ������ �Ǻ��ϴ� �޼���.
	void improvedCalcSumAmount(Mat& coinImg, vector<Vec3f> silverCoins, vector<Vec3f> copperCoins, float& sumAmount);
	// ������ Hue ���� �������� Copper(����) ��������, Silver(��) �������� �����Ͽ� �ΰ��� vector�� ��� �޼���.
	void classificationByColor(Mat hsvImg, vector<Vec3f> coins, vector<Vec3f>& silverCoins, vector<Vec3f>& copperCoins);
};

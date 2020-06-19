//
//  ImageProcessing.h
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

// ������ ��ó��, ��ó���� ����ϴ� Ŭ����
class ImageProcessor
{
private:
	Mat inputImg, grayImg, binaryImg, gaussian, cannyEdge;
public:
	ImageProcessor();
	// inputImg�� �޾ƿ��� ������.
	ImageProcessor(Mat inputImg);	
	// inputImg���� ���� ã�´�.
	vector<Vec3f> findCircles();	
	// ������ ��ġ�� ��� ��(Mask)�� �׸���.
	void makeMasks(Mat& maskImg, vector<Vec3f> coins);		
	// USD ��� ����� ����Ѵ�. 
	void printUSDResult(Mat& outputImg, float sumAmount);
	// KRW ��� ����� ����Ѵ�.
	void printKRWResult(Mat& outputImg, float sumAmount);	
};


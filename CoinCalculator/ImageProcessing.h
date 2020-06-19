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

// 영상의 전처리, 후처리를 담당하는 클래스
class ImageProcessor
{
private:
	Mat inputImg, grayImg, binaryImg, gaussian, cannyEdge;
public:
	ImageProcessor();
	// inputImg를 받아오는 생성자.
	ImageProcessor(Mat inputImg);	
	// inputImg에서 원을 찾는다.
	vector<Vec3f> findCircles();	
	// 원들의 위치에 흰색 원(Mask)을 그린다.
	void makeMasks(Mat& maskImg, vector<Vec3f> coins);		
	// USD 계산 결과를 출력한다. 
	void printUSDResult(Mat& outputImg, float sumAmount);
	// KRW 계산 결과를 출력한다.
	void printKRWResult(Mat& outputImg, float sumAmount);	
};


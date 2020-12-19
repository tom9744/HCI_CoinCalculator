# OpenCV CoinCalculator
#### 2020 Spring Semester, HCI OpenCV Project for detecting USD coins, and calculating the total amount of money.

## 배경
2020년 1학기 인간 컴퓨터 상호작용(HCI) 강의 프로젝트로 진행하였다.
강의를 수강하며 학습한 OpenCV 기반의 영상처리 알고리즘을 실제로 사용해보기 위하여,
머신러닝 관련 기법을 제외하고 순수한 영상처리 기법으로만 접근할 수 있는 주제를 선정하였다.

## 구현 방법
OpenCV에서 제공하는 Image Binarization, Canny Edge Detector 등을 이용해 영상 전처리를 진행하고,
Hough Circle Transform을 이용해 영상 내 존재하는 원형 객체를 검출하는 방법으로 접근하였다. 

이후 cvtColor 메서드를 이용해 BGR 색상공간의 영상을 HSV 색상공간으로 변환하여 Hue Value를 추출하고,
검출된 원형 객체의 반지름을 이용해 동전을 분류하여 최종적으로 영상 내 동전의 가치 합을 산출하였다.

## 구현 결과
한화, 미화 동전에 대한 동전 자동 계수기를 구현하였다.

본 프로그램에서 지원하는 동전은 다음과 같다.
- 한화 500원, 100원, 50원, 10원 동전
- 미화 1달러, 50센트, 25센트, 10센트, 1센트 동전 

![구현 결과물](https://github.com/tom9744/HCI_CoinCalculator/blob/master/result.png)


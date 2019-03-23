#pragma once
#ifndef __BLACKCONTOURS_H__
#define __BLACKCONTOURS_H__

#include <time.h>
#include <stdio.h>
#include <opencv.hpp>

using namespace cv;
using namespace std;
#define CONTOUR_THRESHOLD_MIN 1 //�����ֵ
#define CONTOUR_THRESHOLD_MAX 20000000 //��ͨ������//�����

Mat imagePreprocessing(Mat frame);
bool ascendSort(vector<Point> a, vector<Point> b);
bool descendSort(vector<Point> a, vector<Point> b);
void bwboundaries(Mat detectArea, Mat originalFrame, Rect ROI);//��ͨ����
void crowdHierarchy(int count, float objArea, Mat frame,Mat detectArea,Rect ROI);//ӵ���ȵȼ��ж�
#endif
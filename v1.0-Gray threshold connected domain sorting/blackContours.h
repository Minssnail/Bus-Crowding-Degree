#pragma once
#ifndef __BLACKCONTOURS_H__
#define __BLACKCONTOURS_H__

#include <time.h>
#include <stdio.h>
#include <opencv.hpp>

using namespace cv;
using namespace std;
#define CONTOUR_THRESHOLD_MIN 100 //相对数值
#define CONTOUR_THRESHOLD_MAX 20000 //连通域轮廓//按面积

Mat imagePreprocessing(Mat frame);
bool ascendSort(vector<Point> a, vector<Point> b);
bool descendSort(vector<Point> a, vector<Point> b);
void bwboundaries(Mat detectArea, Mat originalFrame, Rect ROI);//连通域标记
void crowdHierarchy(int count, float objArea, Mat frame);//拥挤度等级判定
#endif
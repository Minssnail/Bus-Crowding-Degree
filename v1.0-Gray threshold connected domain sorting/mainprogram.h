#pragma once
#ifndef __MAINPROGRAM_H__
#define __MAINPROGRAM_H__

#include "opencv.hpp"
#include "windows.h"
#include "stdafx.h"
#include <iostream>  
#include<vector>
#include<algorithm>
using namespace cv;
using namespace std;

#define TESTVIDEO_PATH "E:\\bus_test\\"
//#define TESTVIDEO_FILENAME "bright1.avi "
#define TESTVIDEO_FILENAME "dark.avi "

int **rois;
void select(Mat frame);
void blackContours(VideoCapture capture, int delay, Rect Get_Aear);

#endif




// getROI.cpp :  video.avi to images, and pick up ROI.
//
#include "stdafx.h"
#include <iostream>  
#include<vector>
#include<algorithm>

#define TESTVIDEO_PATH "E:\\bus_test\\"
#define TESTVIDEO_FILENAME "bright1.avi "
#define XLENGTH 420
#define YLENGTH 0
#define WIDTH 350
#define HEIGHT height
#define CONTOUR_THRESHOLD_MIN 100
#define CONTOUR_THRESHOLD_MAX 300
using namespace cv;
using namespace std;



///////////////////////////////////////////////////////////////////////////////////////////////////////
// 方法3函数<1>：图像预处理
Mat imagePreprocessing(Mat frame)
{
	cvtColor(frame, frame, COLOR_BGR2GRAY);	//灰度图转换
	//equalizeHist(frame, frame);		//直方图均衡化
	//blur(frame, frame, Size(3, 3));	//高斯滤波

	return frame;
}

//轮廓按照面积大小升序排序
bool ascendSort(vector<Point> a, vector<Point> b) {
	return a.size() < b.size();

}

//轮廓按照面积大小降序排序
bool descendSort(vector<Point> a, vector<Point> b) {
	return a.size() > b.size();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//图像连通域
void bwboundaries(Mat detectArea, Mat originalFrame)
{
	Mat matSrc;												//用来临时存放图片
	matSrc = detectArea.clone();
	cv::imshow("matSrc0", matSrc);
	Mat matSrc_min = matSrc.clone();
	//CvMemStorage* storage = cvCreateMemStorage(0);			//查找连通体的储存空间
	//CvSeq *contour = 0;										//连通体信息的结构体
	vector<vector<Point> > contours_min;//滤除面积小的连通域
	vector<vector<Point> > contours_max;//滤除面积大的连通域
	vector<vector<Point> > contours;//设定阈值滤除后的连通域
	vector< vector< Point> > contours2; //用于保存面积不足100的轮廓
	vector< vector< Point> > contours3; //用于保存面积大于1000的轮廓
	//vector<Point> tempV;				//暂存的轮廓
	vector<Vec4i> hierarchy;//Vec4i是指每一个vector元素中有四个int型数据。

	//设定阈值去除小面积的连通域
	//轮廓按照面积大小进行升序排序//对一帧图像的每个连通域进行筛选
	findContours(matSrc, contours_min, hierarchy,
		CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	sort(contours_min.begin(), contours_min.end(), ascendSort);//升序排序
	vector<vector<Point> >::iterator itc1 = contours_min.begin();
	while (itc1 != contours_min.end())
	{
		//获得轮廓的矩形边界
		Rect rect1 = boundingRect(*itc1);
		int x = rect1.x;
		int y = rect1.y;
		int w = rect1.width;
		int h = rect1.height;
		if (itc1->size() < CONTOUR_THRESHOLD_MIN||w/h>3 || h / w>3)
		{
			//把轮廓面积不足100的区域，放到容器contours2中
			contours2.push_back(*itc1);
			//删除轮廓面积不足100的区域，即用黑色填充轮廓面积不足100的区域：
			cv::drawContours(matSrc_min, contours2, -1, Scalar(0, 0, 0), CV_FILLED);
		}
		
		//cv::waitKey(100);
		//tempV.clear();
		++itc1;
	}
	cv::imshow("matSrc_min", matSrc_min);

	//删除面积过大的连通域
	Mat matSrc_max = matSrc_min.clone();
	findContours(matSrc_min, contours_max, hierarchy,
		CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	sort(contours_max.begin(), contours_max.end(), descendSort);//升序排序
	vector<vector<Point> >::iterator itc2 = contours_max.begin();
	//int i = 0;
	while (itc2 != contours_max.end())
	{
		//获得轮廓的矩形边界
		Rect rect2 = boundingRect(*itc2);
		int x = rect2.x;
		int y = rect2.y;
		int w = rect2.width;
		int h = rect2.height;
		if (itc2->size() > CONTOUR_THRESHOLD_MAX || w / h>3 || h / w>3)
		{
			//把轮廓面积大于1000的区域，放到容器contours3中，
			contours3.push_back(*itc2);
			//删除轮廓面积大于1000的区域，即用黑色填充轮廓面积大于1000的区域：
			cv::drawContours(matSrc_max, contours3, -1, Scalar(0, 0, 0), CV_FILLED);
		}
		++itc2;
	}
	cv::imshow("matSrc_max", matSrc_max);
	
	//框出连通域
	int peopleCount = 0;
	Mat matSrc_area = matSrc_max.clone();
	findContours(matSrc_area, contours, hierarchy,
		CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	sort(contours.begin(), contours.end(), descendSort);//升序排序
	vector<vector<Point> >::iterator itc3 = contours.begin();
	while (itc3 != contours.end())
	{
		Rect rect3 = boundingRect(*itc3);
		Rect rect4;
		rect4.x = rect3.x + XLENGTH;
		rect4.y = rect3.y + YLENGTH;
		rect4.width = rect3.width;
		rect4.height = rect3.height;
		//绘制轮廓的矩形边界
		cv::rectangle(originalFrame, rect4, { 0, 0, 255 }, 1);
		//imshow("originalFrame", originalFrame);
		++itc3;
		++peopleCount;
	}
	/*
	//计算矩
	vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}
	//计算中心矩:
	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}
	//绘制轮廓
	Mat drawing = Mat::zeros(matSrc.size(), CV_8UC1);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(255);
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
		circle(drawing, mc[i], 4, color, -1, 8, 0);
	}
	imshow("outImage", drawing);
	*/
	string text = "People Count: " + to_string(peopleCount);
	putText(originalFrame, text, Point(50, 60), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//在图片上写文字
	imshow("真实视频框", originalFrame);
	
	waitKey(10);
	destroyAllWindows;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bwboundaries(Mat detectArea, Mat originalFrame);
void thresholdMethod(VideoCapture capture, int delay,Rect Get_Aear)
{
	//2.1- 视频帧读入
	Mat frame, frame_AfterPre, frame_AfterPre_inverse, frame_AfterPreClone;
	Mat frameBin, frameBinClone;
	Mat detectAear, detectAear_Bin;

	while (capture.read(frame))
	{
		if (frame.empty())
			break;
		//2.2- 预处理
		frame_AfterPre = imagePreprocessing(frame);
		frame_AfterPreClone = frame_AfterPre.clone();
		detectAear = frame_AfterPreClone(Get_Aear);//ROI

		//2.3- 二值化（留下黑色像素）
		//cv::subtract(cv::Scalar(255, 255, 255), frame_AfterPre, frame_AfterPre_inverse);
		threshold(frame_AfterPre, frameBin,40, 255, THRESH_BINARY_INV);

		frameBinClone = frameBin.clone();
		detectAear_Bin = frameBinClone(Get_Aear);

		// 以下仅对detectAear_Bin进行处理
		//膨胀
		Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
		dilate(detectAear_Bin, detectAear_Bin, element);
		medianBlur(detectAear_Bin, detectAear_Bin, 11);

		//2.4- 最小外接圆 & 所有imshow操作
		bwboundaries(detectAear_Bin, frame);

		imshow("二值化视频", detectAear_Bin);
		//imshow("实际视频", frame);
		waitKey(delay);
	}
	destroyAllWindows();
}

int main()
{
	//1- 读取视频流
	Mat frame;
	string videoPath = string(TESTVIDEO_PATH) + string(TESTVIDEO_FILENAME);
	VideoCapture capture;
	capture.open(videoPath);
	int width = capture.get(CV_CAP_PROP_FRAME_WIDTH);	//视频帧宽度
	int height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);	//视频帧高度
	printf("%d %d", width, height);
	double rate = capture.get(CV_CAP_PROP_FPS);//获取视频文件的帧率
	int numFrames = capture.get(CV_CAP_PROP_FRAME_COUNT);//获取视频文件的帧数
	int delay = cvRound(1000.000 / rate);	//读取延时
											
	//通道区域
	Rect walkingRoad(XLENGTH, YLENGTH, WIDTH, HEIGHT); //ROI区域

	if (!capture.isOpened())//判断是否打开视频文件
	{
		cout << "视频读取有错！！！" << endl;
		return -1;
	}
	else
	{
		//方法3:论文：二值化 => edge => 最小外接圆（进一步限制）
		thresholdMethod(capture, delay, walkingRoad);
	}
	return 0;
}






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
// ����3����<1>��ͼ��Ԥ����
Mat imagePreprocessing(Mat frame)
{
	cvtColor(frame, frame, COLOR_BGR2GRAY);	//�Ҷ�ͼת��
	//equalizeHist(frame, frame);		//ֱ��ͼ���⻯
	//blur(frame, frame, Size(3, 3));	//��˹�˲�

	return frame;
}

//�������������С��������
bool ascendSort(vector<Point> a, vector<Point> b) {
	return a.size() < b.size();

}

//�������������С��������
bool descendSort(vector<Point> a, vector<Point> b) {
	return a.size() > b.size();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ͼ����ͨ��
void bwboundaries(Mat detectArea, Mat originalFrame)
{
	Mat matSrc;												//������ʱ���ͼƬ
	matSrc = detectArea.clone();
	cv::imshow("matSrc0", matSrc);
	Mat matSrc_min = matSrc.clone();
	//CvMemStorage* storage = cvCreateMemStorage(0);			//������ͨ��Ĵ���ռ�
	//CvSeq *contour = 0;										//��ͨ����Ϣ�Ľṹ��
	vector<vector<Point> > contours_min;//�˳����С����ͨ��
	vector<vector<Point> > contours_max;//�˳���������ͨ��
	vector<vector<Point> > contours;//�趨��ֵ�˳������ͨ��
	vector< vector< Point> > contours2; //���ڱ����������100������
	vector< vector< Point> > contours3; //���ڱ����������1000������
	//vector<Point> tempV;				//�ݴ������
	vector<Vec4i> hierarchy;//Vec4i��ָÿһ��vectorԪ�������ĸ�int�����ݡ�

	//�趨��ֵȥ��С�������ͨ��
	//�������������С������������//��һ֡ͼ���ÿ����ͨ�����ɸѡ
	findContours(matSrc, contours_min, hierarchy,
		CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	sort(contours_min.begin(), contours_min.end(), ascendSort);//��������
	vector<vector<Point> >::iterator itc1 = contours_min.begin();
	while (itc1 != contours_min.end())
	{
		//��������ľ��α߽�
		Rect rect1 = boundingRect(*itc1);
		int x = rect1.x;
		int y = rect1.y;
		int w = rect1.width;
		int h = rect1.height;
		if (itc1->size() < CONTOUR_THRESHOLD_MIN||w/h>3 || h / w>3)
		{
			//�������������100�����򣬷ŵ�����contours2��
			contours2.push_back(*itc1);
			//ɾ�������������100�����򣬼��ú�ɫ��������������100������
			cv::drawContours(matSrc_min, contours2, -1, Scalar(0, 0, 0), CV_FILLED);
		}
		
		//cv::waitKey(100);
		//tempV.clear();
		++itc1;
	}
	cv::imshow("matSrc_min", matSrc_min);

	//ɾ������������ͨ��
	Mat matSrc_max = matSrc_min.clone();
	findContours(matSrc_min, contours_max, hierarchy,
		CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	sort(contours_max.begin(), contours_max.end(), descendSort);//��������
	vector<vector<Point> >::iterator itc2 = contours_max.begin();
	//int i = 0;
	while (itc2 != contours_max.end())
	{
		//��������ľ��α߽�
		Rect rect2 = boundingRect(*itc2);
		int x = rect2.x;
		int y = rect2.y;
		int w = rect2.width;
		int h = rect2.height;
		if (itc2->size() > CONTOUR_THRESHOLD_MAX || w / h>3 || h / w>3)
		{
			//�������������1000�����򣬷ŵ�����contours3�У�
			contours3.push_back(*itc2);
			//ɾ�������������1000�����򣬼��ú�ɫ��������������1000������
			cv::drawContours(matSrc_max, contours3, -1, Scalar(0, 0, 0), CV_FILLED);
		}
		++itc2;
	}
	cv::imshow("matSrc_max", matSrc_max);
	
	//�����ͨ��
	int peopleCount = 0;
	Mat matSrc_area = matSrc_max.clone();
	findContours(matSrc_area, contours, hierarchy,
		CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	sort(contours.begin(), contours.end(), descendSort);//��������
	vector<vector<Point> >::iterator itc3 = contours.begin();
	while (itc3 != contours.end())
	{
		Rect rect3 = boundingRect(*itc3);
		Rect rect4;
		rect4.x = rect3.x + XLENGTH;
		rect4.y = rect3.y + YLENGTH;
		rect4.width = rect3.width;
		rect4.height = rect3.height;
		//���������ľ��α߽�
		cv::rectangle(originalFrame, rect4, { 0, 0, 255 }, 1);
		//imshow("originalFrame", originalFrame);
		++itc3;
		++peopleCount;
	}
	/*
	//�����
	vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}
	//�������ľ�:
	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}
	//��������
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
	putText(originalFrame, text, Point(50, 60), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
	imshow("��ʵ��Ƶ��", originalFrame);
	
	waitKey(10);
	destroyAllWindows;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bwboundaries(Mat detectArea, Mat originalFrame);
void thresholdMethod(VideoCapture capture, int delay,Rect Get_Aear)
{
	//2.1- ��Ƶ֡����
	Mat frame, frame_AfterPre, frame_AfterPre_inverse, frame_AfterPreClone;
	Mat frameBin, frameBinClone;
	Mat detectAear, detectAear_Bin;

	while (capture.read(frame))
	{
		if (frame.empty())
			break;
		//2.2- Ԥ����
		frame_AfterPre = imagePreprocessing(frame);
		frame_AfterPreClone = frame_AfterPre.clone();
		detectAear = frame_AfterPreClone(Get_Aear);//ROI

		//2.3- ��ֵ�������º�ɫ���أ�
		//cv::subtract(cv::Scalar(255, 255, 255), frame_AfterPre, frame_AfterPre_inverse);
		threshold(frame_AfterPre, frameBin,40, 255, THRESH_BINARY_INV);

		frameBinClone = frameBin.clone();
		detectAear_Bin = frameBinClone(Get_Aear);

		// ���½���detectAear_Bin���д���
		//����
		Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
		dilate(detectAear_Bin, detectAear_Bin, element);
		medianBlur(detectAear_Bin, detectAear_Bin, 11);

		//2.4- ��С���Բ & ����imshow����
		bwboundaries(detectAear_Bin, frame);

		imshow("��ֵ����Ƶ", detectAear_Bin);
		//imshow("ʵ����Ƶ", frame);
		waitKey(delay);
	}
	destroyAllWindows();
}

int main()
{
	//1- ��ȡ��Ƶ��
	Mat frame;
	string videoPath = string(TESTVIDEO_PATH) + string(TESTVIDEO_FILENAME);
	VideoCapture capture;
	capture.open(videoPath);
	int width = capture.get(CV_CAP_PROP_FRAME_WIDTH);	//��Ƶ֡���
	int height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);	//��Ƶ֡�߶�
	printf("%d %d", width, height);
	double rate = capture.get(CV_CAP_PROP_FPS);//��ȡ��Ƶ�ļ���֡��
	int numFrames = capture.get(CV_CAP_PROP_FRAME_COUNT);//��ȡ��Ƶ�ļ���֡��
	int delay = cvRound(1000.000 / rate);	//��ȡ��ʱ
											
	//ͨ������
	Rect walkingRoad(XLENGTH, YLENGTH, WIDTH, HEIGHT); //ROI����

	if (!capture.isOpened())//�ж��Ƿ����Ƶ�ļ�
	{
		cout << "��Ƶ��ȡ�д�����" << endl;
		return -1;
	}
	else
	{
		//����3:���ģ���ֵ�� => edge => ��С���Բ����һ�����ƣ�
		thresholdMethod(capture, delay, walkingRoad);
	}
	return 0;
}


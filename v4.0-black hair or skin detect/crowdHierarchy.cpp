//function: determine the crowd 
#include <stdio.h>
#include <opencv.hpp>
#define COUNT_LEVEL1 0
#define COUNT_LEVEL2 1
#define COUNT_LEVEL3 2
#define COUNT_LEVEL4 3
#define AREA_LEVEL1 0
#define AREA_LEVEL2 1
#define AREA_LEVEL3 2
#define AREA_LEVEL4 3
using namespace cv;
using namespace std;


void crowdHierarchy(int count, float objArea, Mat frame,Mat detectArea,Rect ROI)
{
	vector<vector<Point> > contours_bw;//�趨��ֵ�˳������ͨ��
	vector<Vec4i> hierarchy;//Vec4i��ָÿһ��vectorԪ�������ĸ�int�����ݡ�
	findContours(detectArea, contours_bw, hierarchy,
		CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);//, Point(0, 0)
	int x = ROI.x;
	int y = ROI.y;
	//������ͨ������趨ӵ����״̬///////////////////////////////////////////////////////////////////////////////////////
	int count_status=0;
	if (count >= 0 && count <= 1)
		count_status = COUNT_LEVEL1;
	else if (count >= 2 && count <= 3)
		count_status = COUNT_LEVEL2;
	else if (count >= 3 && count <= 4)
		count_status = COUNT_LEVEL3;
	else if (count >= 5)
		count_status = COUNT_LEVEL4;
	//������ͨ�����ռ���趨ӵ����״̬//////////////////////////////////////////////////////////////////////////////////////
	int objArea_status=0;
	if (objArea >= 0 && objArea < 10)
		objArea_status = AREA_LEVEL1;
	else if (objArea >= 10 && objArea < 20)
		objArea_status = AREA_LEVEL2;
	else if (objArea >= 20 && objArea < 30)
		objArea_status = AREA_LEVEL3;
	else if (objArea >= 40)
		objArea_status = AREA_LEVEL4;
	printf("count_status:%d objArea_status:%d\n", count_status,objArea_status);
	//�����ж�ӵ���ȵȼ�/////////////////////////////////////////////////////////////////////////////////////////////////////
	int Hierarchy=0;
	switch (count_status)
	{
	case COUNT_LEVEL1:
		if (objArea_status == AREA_LEVEL1)
		{
			Hierarchy = 0;
			string text1 = "Not crowded ";
			putText(frame, text1, Point(25, 50), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			string text2 = "Hierarchy: " + to_string(Hierarchy);
			putText(frame, text2, Point(25, 120), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			drawContours(frame, contours_bw, -1, Scalar(0, 255, 0), CV_RETR_TREE, CV_CHAIN_APPROX_NONE, hierarchy, INT_MAX, Point(x, y));
			imshow("��ʵ��Ƶ��", frame);

		}
		else if (objArea_status == AREA_LEVEL2)
		{
			Hierarchy = 1;
			string text1 = "Mild crowding";
			putText(frame, text1, Point(25, 50), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			string text2 = "Hierarchy: " + to_string(Hierarchy);
			putText(frame, text2, Point(25, 120), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			drawContours(frame, contours_bw, -1, Scalar(0, 255, 0), CV_RETR_TREE, CV_CHAIN_APPROX_NONE, hierarchy, INT_MAX, Point(x, y));
			imshow("��ʵ��Ƶ��", frame);
		}
		else if (objArea_status == AREA_LEVEL3)
		{
			Hierarchy = 2;
			string text1 = "Moderate crowding";
			putText(frame, text1, Point(25, 50), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			string text2 = "Hierarchy: " + to_string(Hierarchy);
			putText(frame, text2, Point(25, 120), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			drawContours(frame, contours_bw, -1, Scalar(0, 255, 0), CV_RETR_TREE, CV_CHAIN_APPROX_NONE, hierarchy, INT_MAX, Point(x, y));
			imshow("��ʵ��Ƶ��", frame);
		}
		else if (objArea_status == AREA_LEVEL4)
		{
			Hierarchy = 3;
			string text1 = "Severe crowding";
			putText(frame, text1, Point(25, 50), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			string text2 = "Hierarchy: " + to_string(Hierarchy);
			putText(frame, text2, Point(25, 120), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			drawContours(frame, contours_bw, -1, Scalar(0, 255, 0), CV_RETR_TREE, CV_CHAIN_APPROX_NONE, hierarchy, INT_MAX, Point(x, y));
			imshow("��ʵ��Ƶ��", frame);
		}
		break;
	//------------------------------------------------------------------------------------------------------------------//
	case COUNT_LEVEL2:
		if (objArea_status == AREA_LEVEL1)
		{
			Hierarchy = 0;
			string text1 = "Not crowded ";
			putText(frame, text1, Point(25, 50), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			string text2 = "Hierarchy: " + to_string(Hierarchy);
			putText(frame, text2, Point(25, 120), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			drawContours(frame, contours_bw, -1, Scalar(0, 255, 0), CV_RETR_TREE, CV_CHAIN_APPROX_NONE, hierarchy, INT_MAX, Point(x, y));
			imshow("��ʵ��Ƶ��", frame);
		}
		else if (objArea_status == AREA_LEVEL2)
		{
			Hierarchy = 1;
			string text1 = "Mild crowding";
			putText(frame, text1, Point(25, 50), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			string text2 = "Hierarchy: " + to_string(Hierarchy);
			putText(frame, text2, Point(25, 120), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			drawContours(frame, contours_bw, -1, Scalar(0, 255, 0), CV_RETR_TREE, CV_CHAIN_APPROX_NONE, hierarchy, INT_MAX, Point(x, y));
			imshow("��ʵ��Ƶ��", frame);
		}
		else if (objArea_status == AREA_LEVEL3)
		{
			Hierarchy = 2;
			string text1 = "Moderate crowding";
			putText(frame, text1, Point(25, 50), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			string text2 = "Hierarchy: " + to_string(Hierarchy);
			putText(frame, text2, Point(25, 120), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			drawContours(frame, contours_bw, -1, Scalar(0, 255, 0), CV_RETR_TREE, CV_CHAIN_APPROX_NONE, hierarchy, INT_MAX, Point(x, y));
			imshow("��ʵ��Ƶ��", frame);
		}
		else if (objArea_status == AREA_LEVEL4)
		{
			Hierarchy = 3;
			string text1 = "Severe crowding";
			putText(frame, text1, Point(25, 50), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			string text2 = "Hierarchy: " + to_string(Hierarchy);
			putText(frame, text2, Point(25, 120), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			drawContours(frame, contours_bw, -1, Scalar(0, 255, 0), CV_RETR_TREE, CV_CHAIN_APPROX_NONE, hierarchy, INT_MAX, Point(x, y));
			imshow("��ʵ��Ƶ��", frame);
		}
		break;
	//------------------------------------------------------------------------------------------------------------------//
	case COUNT_LEVEL3:
		if (objArea_status == AREA_LEVEL1)
		{
			Hierarchy = 0;
			string text1 = "Not crowded ";
			putText(frame, text1, Point(25, 50), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			string text2 = "Hierarchy: " + to_string(Hierarchy);
			putText(frame, text2, Point(25, 120), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			drawContours(frame, contours_bw, -1, Scalar(0, 255, 0), CV_RETR_TREE, CV_CHAIN_APPROX_NONE, hierarchy, INT_MAX, Point(x, y));
			imshow("��ʵ��Ƶ��", frame);
		}
		else if (objArea_status == AREA_LEVEL2)
		{
			Hierarchy = 1;
			string text1 = "Mild crowding";
			putText(frame, text1, Point(25, 50), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			string text2 = "Hierarchy: " + to_string(Hierarchy);
			putText(frame, text2, Point(25, 120), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			drawContours(frame, contours_bw, -1, Scalar(0, 255, 0), CV_RETR_TREE, CV_CHAIN_APPROX_NONE, hierarchy, INT_MAX, Point(x, y));
			imshow("��ʵ��Ƶ��", frame);
		}
		else if (objArea_status == AREA_LEVEL3)
		{
			Hierarchy = 2;
			string text1 = "Moderate crowding";
			putText(frame, text1, Point(25, 50), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			string text2 = "Hierarchy: " + to_string(Hierarchy);
			putText(frame, text2, Point(25, 120), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			drawContours(frame, contours_bw, -1, Scalar(0, 255, 0), CV_RETR_TREE, CV_CHAIN_APPROX_NONE, hierarchy, INT_MAX, Point(x, y));
			imshow("��ʵ��Ƶ��", frame);
		}
		else if (objArea_status == AREA_LEVEL4)
		{
			Hierarchy = 3;
			string text1 = "Severe crowding";
			putText(frame, text1, Point(25, 50), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			string text2 = "Hierarchy: " + to_string(Hierarchy);
			putText(frame, text2, Point(25, 120), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			drawContours(frame, contours_bw, -1, Scalar(0, 255, 0),CV_RETR_TREE, CV_CHAIN_APPROX_NONE, hierarchy,INT_MAX, Point(x,y));
			imshow("��ʵ��Ƶ��", frame);
		}
		break;
	//------------------------------------------------------------------------------------------------------------------//
	case COUNT_LEVEL4:
		if (objArea_status == AREA_LEVEL1)
		{
			Hierarchy = 1;
			string text = "Mild crowding" ;
			putText(frame, text, Point(25, 50), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			string text2 = "Hierarchy: " + to_string(Hierarchy);
			putText(frame, text2, Point(25, 120), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			drawContours(frame, contours_bw, -1, Scalar(0, 255, 0), CV_RETR_TREE, CV_CHAIN_APPROX_NONE, hierarchy, INT_MAX, Point(x, y));
			imshow("��ʵ��Ƶ��", frame);
		}
		else if (objArea_status == AREA_LEVEL2)
		{
			Hierarchy = 2;
			string text = "Moderate crowding" ;
			putText(frame, text, Point(25, 60), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			string text2 = "Hierarchy: " + to_string(Hierarchy);
			putText(frame, text2, Point(25, 120), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			drawContours(frame, contours_bw, -1, Scalar(0, 255, 0), CV_RETR_TREE, CV_CHAIN_APPROX_NONE, hierarchy, INT_MAX, Point(x, y));
			imshow("��ʵ��Ƶ��", frame);
		}
		else if (objArea_status == AREA_LEVEL3)
		{
			Hierarchy = 3;
			string text = "Severe crowding" ;
			putText(frame, text, Point(25, 60), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			string text2 = "Hierarchy: " + to_string(Hierarchy);
			putText(frame, text2, Point(25, 120), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			drawContours(frame, contours_bw, -1, Scalar(0, 255, 0), CV_RETR_TREE, CV_CHAIN_APPROX_NONE, hierarchy, INT_MAX, Point(x, y));
			imshow("��ʵ��Ƶ��", frame);
		}
		else if (objArea_status == AREA_LEVEL4)
		{
			Hierarchy = 3;
			string text = "Severe crowding" ;
			putText(frame, text, Point(25, 60), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			string text2 = "Hierarchy: " + to_string(Hierarchy);
			putText(frame, text2, Point(25, 120), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 4, 8);//��ͼƬ��д����
			drawContours(frame, contours_bw, -1, Scalar(0, 255, 0), CV_RETR_TREE, CV_CHAIN_APPROX_NONE, hierarchy, INT_MAX, Point(x, y));
			imshow("��ʵ��Ƶ��", frame);
		}
		break;
	default:
		printf("ӵ���ȵȼ������ڣ�������������Ƶ\n");
	}
	printf("Hierarchy:%d \n", Hierarchy);
}

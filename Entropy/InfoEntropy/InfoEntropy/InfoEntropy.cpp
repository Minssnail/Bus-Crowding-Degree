#include "opencv.hpp"
#include <iostream>  

#define uchar unsigned char

using namespace cv;
using namespace std;


double Entropy(Mat img);

int main()
{
	Mat Crowded_srcImage = imread("E:\\������Ŀ\\1������\\3ӵ����\\project\\readVideo\\Crowded\\image369.jpg");
	Mat NotCrowded_srcImage = imread("E:\\������Ŀ\\1������\\3ӵ����\\project\\readVideo\\NotCrowded\\image292.jpg");
	
	double Crowded_InfoEn = Entropy(Crowded_srcImage);
	double NotCrowded_InfoEn = Entropy(NotCrowded_srcImage);
	printf("ӵ��ʱ����Ϣ�أ�%f\n", Crowded_InfoEn);
	printf("��ӵ��ʱ����Ϣ�أ�%f\n", NotCrowded_InfoEn);
}


double Entropy(Mat img)
{

	//�����ڴ�-��Ҷ�ֱ��ͼ
	double temp[256] = { 0};

	// ����ÿ�����ص��ۻ�ֵ
	for (int m = 0; m < img.rows; m++)
	{// ��Ч�������еķ�ʽ
		const uchar* t = img.ptr<uchar>(m);
		for (int n = 0; n < img.cols; n++)
		{
			int i = t[n];
			temp[i] = temp[i] + 1;
		}
	}

	// ����ÿ������ֵ�ĸ���
	for (int i = 0; i < 256; i++)
	{
		temp[i] = temp[i] / (img.rows*img.cols);
	}

	double result = 0;
	// ����ͼ����Ϣ��
	for (int i = 0; i < 256; i++)
	{
		if (temp[i] == 0.0)
			result = result;
		else
			result = result - temp[i] * (log(temp[i]) / log(2.0));
	}

	return result;

}

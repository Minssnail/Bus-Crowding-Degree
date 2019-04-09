#include "opencv.hpp"
#include <iostream>  

#define uchar unsigned char

using namespace cv;
using namespace std;


double Entropy(Mat img);

int main()
{
	Mat Crowded_srcImage = imread("E:\\科研项目\\1公交车\\3拥挤度\\project\\readVideo\\Crowded\\image369.jpg");
	Mat NotCrowded_srcImage = imread("E:\\科研项目\\1公交车\\3拥挤度\\project\\readVideo\\NotCrowded\\image292.jpg");
	
	double Crowded_InfoEn = Entropy(Crowded_srcImage);
	double NotCrowded_InfoEn = Entropy(NotCrowded_srcImage);
	printf("拥挤时的信息熵：%f\n", Crowded_InfoEn);
	printf("不拥挤时的信息熵：%f\n", NotCrowded_InfoEn);
}


double Entropy(Mat img)
{

	//开辟内存-存灰度直方图
	double temp[256] = { 0};

	// 计算每个像素的累积值
	for (int m = 0; m < img.rows; m++)
	{// 有效访问行列的方式
		const uchar* t = img.ptr<uchar>(m);
		for (int n = 0; n < img.cols; n++)
		{
			int i = t[n];
			temp[i] = temp[i] + 1;
		}
	}

	// 计算每个像素值的概率
	for (int i = 0; i < 256; i++)
	{
		temp[i] = temp[i] / (img.rows*img.cols);
	}

	double result = 0;
	// 计算图像信息熵
	for (int i = 0; i < 256; i++)
	{
		if (temp[i] == 0.0)
			result = result;
		else
			result = result - temp[i] * (log(temp[i]) / log(2.0));
	}

	return result;

}

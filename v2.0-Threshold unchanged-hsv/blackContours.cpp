#include"blackContours.h"

void blackContours(VideoCapture capture, int delay, Rect Get_Area)
{
	//2.1- 视频帧读入
	Mat frame, frame_Clone, frame_AfterPre_inverse, frame_AfterPreClone;
	Mat dst_bw;
	Mat frameBin, frameBinClone;
	Mat detectArea, detectArea_AfterPre, detectArea_hsv;

	while (capture.read(frame))
	{
		if (frame.empty())
			break;

		//2.2- 预处理:对ROI转为HSV
		frame_Clone = frame.clone();
		detectArea = frame_Clone(Get_Area);//ROI-bgr
		detectArea_AfterPre = imagePreprocessing(detectArea);
		
		//2.3- 二值化（留下黑色像素）/对彩色图像进行处理：H低，V低的近黑像素点存入dst_bw作为前景
		detectArea_hsv = detectArea_AfterPre.clone();//hsv图像
		//色相
		int hmin = 10;
		int hmin_Max = 360;
		int hmax = 60;
		int hmax_Max = 360;
		//饱和度
		int smin = 0;
		int smin_Max = 255;
		int smax = 255;
		int smax_Max = 255;
		//亮度
		int vmin = 0;
		int vmin_Max = 255;
		int vmax =255;
		int vmax_Max = 255;
		
		//hsv分割二值图dst_bw
		Mat mask;
		inRange(detectArea_hsv, Scalar(hmin, smin / float(smin_Max), vmin / float(vmin_Max)),
			Scalar(hmax, smax / float(smax_Max), vmax / float(vmax_Max)), mask);
		
		Mat detectArea_Bin = Mat::zeros(detectArea_hsv.size(), CV_8UC1);
		//detectArea_Bin = dst_bw.clone();
		//只保留mask=255满足色相和亮度接近黑色部分
		for (int r = 0; r < detectArea_hsv.rows; r++)
		{
			for (int c = 0; c < detectArea_hsv.cols; c++)
			{
				if (mask.at<uchar>(r, c) == 255)
				{
					detectArea_Bin.at<uchar>(r, c) = 255;
				}
			}
		}
		//输出图像
		imshow("hsv图像二值化分割", detectArea_Bin);
	

		// 以下仅对detectArea_Bin进行处理
		//膨胀
		Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
		dilate(detectArea_Bin, detectArea_Bin, element);
		medianBlur(detectArea_Bin, detectArea_Bin, 11);

		//所有imshow操作
		bwboundaries(detectArea_Bin, frame, Get_Area);

		//imshow("二值化视频", detectArea_Bin);
		//imshow("实际视频", frame);
		waitKey(delay);
	}
	destroyAllWindows();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
// 方法3函数<1>：图像预处理
Mat imagePreprocessing(Mat frame)
{
	//彩色图像的灰度值归一化  
	frame.convertTo(frame, CV_32FC3, 1.0 / 255, 0);
	cvtColor(frame, frame, COLOR_BGR2HSV);	//RGB转HSV
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
void bwboundaries(Mat detectArea, Mat originalFrame , Rect ROI)
{
	Mat matSrc;												//用来临时存放图片
	matSrc = detectArea.clone();
	//cv::imshow("matSrc0", matSrc);
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
		//if (itc1->size() < CONTOUR_THRESHOLD_MIN || w / h>3 || h / w>3)
		if (itc1->size() < CONTOUR_THRESHOLD_MIN)
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
	cv::imshow("去除小面积连通域的二值图", matSrc_min);

	//删除面积过大的连通域
	Mat matSrc_max = matSrc_min.clone();
	findContours(matSrc_min, contours_max, hierarchy,
		CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	sort(contours_max.begin(), contours_max.end(), descendSort);//升序排序
	vector<vector<Point> >::iterator itc2 = contours_max.begin();
	while (itc2 != contours_max.end())
	{
		//获得轮廓的矩形边界
		Rect rect2 = boundingRect(*itc2);
		int x = rect2.x;
		int y = rect2.y;
		int w = rect2.width;
		int h = rect2.height;
		//if (itc2->size() > CONTOUR_THRESHOLD_MAX || w / h>3 || h / w>3)
		if (itc2->size() > CONTOUR_THRESHOLD_MAX )
		{
			//把轮廓面积大于1000的区域，放到容器contours3中
			contours3.push_back(*itc2);
			//删除轮廓面积大于1000的区域，即用黑色填充轮廓面积大于1000的区域：
			cv::drawContours(matSrc_max, contours3, -1, Scalar(0, 0, 0), CV_FILLED);
		}
		++itc2;
	}
	cv::imshow("去除大面积连通域的二值图", matSrc_max);

	//框出连通域
	int peopleCount = 0;
	Mat matSrc_area = matSrc_max.clone();
	findContours(matSrc_area, contours, hierarchy,
		CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	//获取连通域的个数并框出连通域
	sort(contours.begin(), contours.end(), descendSort);//升序排序
	vector<vector<Point> >::iterator itc3 = contours.begin();
	while (itc3 != contours.end())
	{
		Rect rect3 = boundingRect(*itc3);
		Rect rect4;
		rect4.x = rect3.x + ROI.x;
		rect4.y = rect3.y + ROI.y;
		rect4.width = rect3.width;
		rect4.height = rect3.height;
		//绘制轮廓的矩形边界
		cv::rectangle(originalFrame, rect4, { 0, 0, 255 }, 1);
		//imshow("originalFrame", originalFrame);
		++itc3;
		++peopleCount;
	}
	//获取所有连通域的总面积
	Mat Imglabels, Imgstats, Imgcentriods;
	float countorsArea = 0;
	int Imglabelnum = connectedComponentsWithStats(matSrc_area, Imglabels, Imgstats, Imgcentriods);
	for (int i = 1; i <= peopleCount; i++)//peopleCount=sizeof(Imglabels)?
	{
		countorsArea= countorsArea+Imgstats.at<int>(i, CC_STAT_AREA); //迭代计算连通域的总面积
	}
	float matSrc_size = (float)(matSrc_area.rows*matSrc_area.cols);
	float countorsArea_Percentage = (countorsArea / matSrc_size)*100;
	printf("countorsArea_Percentage:%f,counterArea:%f,matSrc_size:%f\n", countorsArea_Percentage, countorsArea, matSrc_size);
	Imglabels.release();
	Imgstats.release();
	Imgcentriods.release();

	/*method 1 to get the Center of gravity//////////////////////////////////////////////////////////////
	double left = Imgstats.at<int>(i, CC_STAT_LEFT); //连通域的boundingbox的最左边
	double top = Imgstats.at<int>(i, CC_STAT_TOP);//连通域的boundingbox的最上边
	double width = Imgstats.at<int>(i, CC_STAT_WIDTH);//连通域的宽
	double height Imgstats.at<int>(i, CC_STAT_HEIGHT);//连通域的高
	vector<Point2f> pt;  //连通域的重心//pt就是重心
	pt.x = Imgcentriods.at<double>(i, 0);
	pt.y = Imgcentriods.at<double>(i, 1);
	int labelNum = Imgcentriods.rows;//连通域的个数
	*/
	/*method 2 to get the Central point/////////////////////////////////////////////////////////////////
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

	//计算拥挤度等级并在图片上输出拥挤度信息
	crowdHierarchy(peopleCount, countorsArea_Percentage,originalFrame, detectArea,ROI);

	waitKey(10);
	destroyAllWindows;
}

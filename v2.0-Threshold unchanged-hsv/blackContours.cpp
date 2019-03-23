#include"blackContours.h"

void blackContours(VideoCapture capture, int delay, Rect Get_Area)
{
	//2.1- ��Ƶ֡����
	Mat frame, frame_Clone, frame_AfterPre_inverse, frame_AfterPreClone;
	Mat dst_bw;
	Mat frameBin, frameBinClone;
	Mat detectArea, detectArea_AfterPre, detectArea_hsv;

	while (capture.read(frame))
	{
		if (frame.empty())
			break;

		//2.2- Ԥ����:��ROIתΪHSV
		frame_Clone = frame.clone();
		detectArea = frame_Clone(Get_Area);//ROI-bgr
		detectArea_AfterPre = imagePreprocessing(detectArea);
		
		//2.3- ��ֵ�������º�ɫ���أ�/�Բ�ɫͼ����д���H�ͣ�V�͵Ľ������ص����dst_bw��Ϊǰ��
		detectArea_hsv = detectArea_AfterPre.clone();//hsvͼ��
		//ɫ��
		int hmin = 10;
		int hmin_Max = 360;
		int hmax = 60;
		int hmax_Max = 360;
		//���Ͷ�
		int smin = 0;
		int smin_Max = 255;
		int smax = 255;
		int smax_Max = 255;
		//����
		int vmin = 0;
		int vmin_Max = 255;
		int vmax =255;
		int vmax_Max = 255;
		
		//hsv�ָ��ֵͼdst_bw
		Mat mask;
		inRange(detectArea_hsv, Scalar(hmin, smin / float(smin_Max), vmin / float(vmin_Max)),
			Scalar(hmax, smax / float(smax_Max), vmax / float(vmax_Max)), mask);
		
		Mat detectArea_Bin = Mat::zeros(detectArea_hsv.size(), CV_8UC1);
		//detectArea_Bin = dst_bw.clone();
		//ֻ����mask=255����ɫ������Ƚӽ���ɫ����
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
		//���ͼ��
		imshow("hsvͼ���ֵ���ָ�", detectArea_Bin);
	

		// ���½���detectArea_Bin���д���
		//����
		Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
		dilate(detectArea_Bin, detectArea_Bin, element);
		medianBlur(detectArea_Bin, detectArea_Bin, 11);

		//����imshow����
		bwboundaries(detectArea_Bin, frame, Get_Area);

		//imshow("��ֵ����Ƶ", detectArea_Bin);
		//imshow("ʵ����Ƶ", frame);
		waitKey(delay);
	}
	destroyAllWindows();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
// ����3����<1>��ͼ��Ԥ����
Mat imagePreprocessing(Mat frame)
{
	//��ɫͼ��ĻҶ�ֵ��һ��  
	frame.convertTo(frame, CV_32FC3, 1.0 / 255, 0);
	cvtColor(frame, frame, COLOR_BGR2HSV);	//RGBתHSV
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
void bwboundaries(Mat detectArea, Mat originalFrame , Rect ROI)
{
	Mat matSrc;												//������ʱ���ͼƬ
	matSrc = detectArea.clone();
	//cv::imshow("matSrc0", matSrc);
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
		//if (itc1->size() < CONTOUR_THRESHOLD_MIN || w / h>3 || h / w>3)
		if (itc1->size() < CONTOUR_THRESHOLD_MIN)
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
	cv::imshow("ȥ��С�����ͨ��Ķ�ֵͼ", matSrc_min);

	//ɾ������������ͨ��
	Mat matSrc_max = matSrc_min.clone();
	findContours(matSrc_min, contours_max, hierarchy,
		CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	sort(contours_max.begin(), contours_max.end(), descendSort);//��������
	vector<vector<Point> >::iterator itc2 = contours_max.begin();
	while (itc2 != contours_max.end())
	{
		//��������ľ��α߽�
		Rect rect2 = boundingRect(*itc2);
		int x = rect2.x;
		int y = rect2.y;
		int w = rect2.width;
		int h = rect2.height;
		//if (itc2->size() > CONTOUR_THRESHOLD_MAX || w / h>3 || h / w>3)
		if (itc2->size() > CONTOUR_THRESHOLD_MAX )
		{
			//�������������1000�����򣬷ŵ�����contours3��
			contours3.push_back(*itc2);
			//ɾ�������������1000�����򣬼��ú�ɫ��������������1000������
			cv::drawContours(matSrc_max, contours3, -1, Scalar(0, 0, 0), CV_FILLED);
		}
		++itc2;
	}
	cv::imshow("ȥ���������ͨ��Ķ�ֵͼ", matSrc_max);

	//�����ͨ��
	int peopleCount = 0;
	Mat matSrc_area = matSrc_max.clone();
	findContours(matSrc_area, contours, hierarchy,
		CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	//��ȡ��ͨ��ĸ����������ͨ��
	sort(contours.begin(), contours.end(), descendSort);//��������
	vector<vector<Point> >::iterator itc3 = contours.begin();
	while (itc3 != contours.end())
	{
		Rect rect3 = boundingRect(*itc3);
		Rect rect4;
		rect4.x = rect3.x + ROI.x;
		rect4.y = rect3.y + ROI.y;
		rect4.width = rect3.width;
		rect4.height = rect3.height;
		//���������ľ��α߽�
		cv::rectangle(originalFrame, rect4, { 0, 0, 255 }, 1);
		//imshow("originalFrame", originalFrame);
		++itc3;
		++peopleCount;
	}
	//��ȡ������ͨ��������
	Mat Imglabels, Imgstats, Imgcentriods;
	float countorsArea = 0;
	int Imglabelnum = connectedComponentsWithStats(matSrc_area, Imglabels, Imgstats, Imgcentriods);
	for (int i = 1; i <= peopleCount; i++)//peopleCount=sizeof(Imglabels)?
	{
		countorsArea= countorsArea+Imgstats.at<int>(i, CC_STAT_AREA); //����������ͨ��������
	}
	float matSrc_size = (float)(matSrc_area.rows*matSrc_area.cols);
	float countorsArea_Percentage = (countorsArea / matSrc_size)*100;
	printf("countorsArea_Percentage:%f,counterArea:%f,matSrc_size:%f\n", countorsArea_Percentage, countorsArea, matSrc_size);
	Imglabels.release();
	Imgstats.release();
	Imgcentriods.release();

	/*method 1 to get the Center of gravity//////////////////////////////////////////////////////////////
	double left = Imgstats.at<int>(i, CC_STAT_LEFT); //��ͨ���boundingbox�������
	double top = Imgstats.at<int>(i, CC_STAT_TOP);//��ͨ���boundingbox�����ϱ�
	double width = Imgstats.at<int>(i, CC_STAT_WIDTH);//��ͨ��Ŀ�
	double height Imgstats.at<int>(i, CC_STAT_HEIGHT);//��ͨ��ĸ�
	vector<Point2f> pt;  //��ͨ�������//pt��������
	pt.x = Imgcentriods.at<double>(i, 0);
	pt.y = Imgcentriods.at<double>(i, 1);
	int labelNum = Imgcentriods.rows;//��ͨ��ĸ���
	*/
	/*method 2 to get the Central point/////////////////////////////////////////////////////////////////
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

	//����ӵ���ȵȼ�����ͼƬ�����ӵ������Ϣ
	crowdHierarchy(peopleCount, countorsArea_Percentage,originalFrame, detectArea,ROI);

	waitKey(10);
	destroyAllWindows;
}

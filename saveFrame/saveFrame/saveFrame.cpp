#include<cstring>  
#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

int main()
{
	CvCapture *capture;
	IplImage *nFrames = NULL;
	IplImage *frame;
	capture = cvCreateFileCapture("E:\\科研项目\\1公交车\\公交车厢中部实拍-拥挤度\\bright\\bestAngle.mp4");
	//capture = cvCreateFileCapture("E:\\科研项目\\1公交车\\公交车厢中部实拍-拥挤度\\dark\\nice.mp4");

	assert(capture != NULL);

	int frameH = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
	int frameW = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
	int fps = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	int numFrames = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
	printf("vedio's \nwidth = %d\t height = %d\n video's fps = %d\t numFrames = %d\n", frameW, frameH, fps, numFrames);

	
	cvNamedWindow("camera", 1);
	
	int n = 1, m = numFrames;
	char *cstr = new char[m];
	
	while (m--)
		{
		frame = cvQueryFrame(capture);
		if (!frame)
		break;
		
		sprintf(cstr, "%s%d%s", "E:\\科研项目\\1公交车\\3拥挤度\\project\\readVideo\\NotCrowded\\image", n++, ".jpg");
		//sprintf(cstr, "%s%d%s", "E:\\科研项目\\1公交车\\3拥挤度\\project\\readVideo\\Crowded\\image", n++, ".jpg");

		cvShowImage("camera", frame);
		
		cvSaveImage(cstr, frame);
		
		if (cvWaitKey(33) >= 0)
		break;
		 }
	cvReleaseCapture(&capture);
	cvReleaseImage(&frame);
	cvDestroyWindow("camera");
	
		return 0;
	}
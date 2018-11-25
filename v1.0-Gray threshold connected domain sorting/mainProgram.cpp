#include"mainprogram.h"
constexpr int ROICOUNT = 1;
int main()
{
	//1- Read video stream
	Mat frame;
	string videoPath = string(TESTVIDEO_PATH) + string(TESTVIDEO_FILENAME);
	VideoCapture capture;
	capture.open(videoPath);
	/*
	int width = capture.get(CV_CAP_PROP_FRAME_WIDTH);	//Frame width
	int height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);	//Frame Height
	printf("%d %d", width, height);
	int numFrames = capture.get(CV_CAP_PROP_FRAME_COUNT);//Frame numbers
	*/
	double rate = capture.get(CV_CAP_PROP_FPS);//Get frame rate
	int delay = cvRound(1000.000 / rate);	//Read delay

	if (!capture.isOpened())//Determine whether to open the video file
	{
		cout << "视频读取有错！！！" << endl;
		return -1;
	}
	else
	{
		if (capture.read(frame))//Read the first frame
		{
			select(frame);
			cv::waitKey();
		}
		Rect walkingRoad;//Get ROI
		walkingRoad.x = rois[0][0];
		walkingRoad.y = rois[0][1];
		walkingRoad.width = rois[0][2];
		walkingRoad.height = rois[0][3];
		blackContours(capture, delay, walkingRoad);//Get counters
	}
	capture.release();
	return 0;
}

void select(Mat frame)
{
	Mat frame_mat;
	frame_mat = frame.clone();
	vector<Rect> bboxes;

	rois = new int *[ROICOUNT];
	uint i, j;
	for (i = 0; i < ROICOUNT; i++)
	{
		rois[i] = new int[4];
		for (j = 0; j < 4; j++)
		{
			rois[i][j] = 0;
		}
	}
	selectROIs("第一帧选ROI", frame_mat, bboxes, false);
	cvDestroyWindow("第一帧选ROI");

	if (bboxes.size() < 1)
		return;
	Rect rectROI;

	for (i = 0; i < ROICOUNT; i++)
	{
		if (i >= bboxes.size())
			return;
		rectROI = bboxes.at(i);
		rois[i][0] = rectROI.x;
		rois[i][1] = rectROI.y;
		rois[i][2] = rectROI.width;
		rois[i][3] = rectROI.height;
	}
}



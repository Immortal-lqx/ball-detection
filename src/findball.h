#pragma once
#include <opencv2/opencv.hpp>
class findball
{
public:
	findball();
	void getpicture();
	void creatbar();
	void setparam(int* sin, int* sax);
	void process();
	void draw();
	void show();
	
	static cv::VideoCapture capture;

private:
	int bgr_min[3];
	int bgr_max[3];

	cv::Mat src;
	cv::Mat dst;

	static void on_change(int, void*);
};
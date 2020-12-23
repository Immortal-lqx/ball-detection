#include "findball.h"

findball::findball()
{
	bgr_min[0] = 68; bgr_min[1] = 148; bgr_min[2] = 153;
	bgr_max[0] = 208; bgr_max[1] = 255; bgr_max[2] = 255;		
	//这些数据不知道放哪儿比较好...
}

void findball::getpicture()
{
	cv::Mat frame;
	capture >> frame;
	frame.copyTo(src);
}

void findball::on_change(int, void*)
{

}

void findball::creatbar()
{
	cv::namedWindow("【参数调整】");
	cv::createTrackbar("minB", "【参数调整】", &bgr_min[0], 255, on_change);
	cv::createTrackbar("minG", "【参数调整】", &bgr_min[1], 255, on_change);
	cv::createTrackbar("minR", "【参数调整】", &bgr_min[2], 255, on_change);
	cv::createTrackbar("maxB", "【参数调整】", &bgr_max[0], 255, on_change);
	cv::createTrackbar("maxG", "【参数调整】", &bgr_max[1], 255, on_change);
	cv::createTrackbar("maxR", "【参数调整】", &bgr_max[2], 255, on_change);
	on_change(0, 0);
}

void findball::setparam(int* sin, int* sax)
{
	for (int i = 0; i < 3; i++)
	{
		bgr_min[i] = sin[i];
		bgr_max[i] = sax[i];
	}
}

void findball::process()
{
	//双边滤波
	bilateralFilter(src, dst, 25, 1000, 1000);
	//进行阈值化
	inRange(dst, cv::Scalar(bgr_min[0], bgr_min[1], bgr_min[2]), cv::Scalar(bgr_max[0], bgr_max[1], bgr_max[2]), dst);
	//膨胀
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));
	dilate(dst, dst, element);
}

void findball::draw()
{
	//查找轮廓并筛选矩形
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	std::vector<cv::Rect> rects;

	cv::findContours(dst, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);	//查找轮廓

	for (int i = 0; i < contours.size(); i++)										//存入并筛选矩形
	{
		rects.push_back(cv::boundingRect(contours[i]));
		//注：这里没有使用旋转矩阵		RotatedRect		minAreaRect()
		if ((rects.back().height < 100 || rects.back().width < 100) ||
			fabsf(rects.back().height - rects.back().width) > 20)
			rects.erase(rects.end() - 1);
	}

	//找出圆心并且画出圆
	for (int i = 0; i < rects.size(); i++)
	{
		double R = (rects[i].height + rects[i].width) / 4;
		double x = rects[i].x + R;
		double y = rects[i].y + R;
		cv::Point center(x, y);
		circle(src, center, R + 5, cv::Scalar(0, 0, 255), 5);
	}
}

void findball::show()
{
	cv::imshow("FINALLY", src);
	cv::waitKey(30);
}
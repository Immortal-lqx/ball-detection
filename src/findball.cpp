#include "findball.h"

findball::findball()
{
	bgr_min[0] = 68; bgr_min[1] = 148; bgr_min[2] = 153;
	bgr_max[0] = 208; bgr_max[1] = 255; bgr_max[2] = 255;		
	//��Щ���ݲ�֪�����Ķ��ȽϺ�...
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
	cv::namedWindow("������������");
	cv::createTrackbar("minB", "������������", &bgr_min[0], 255, on_change);
	cv::createTrackbar("minG", "������������", &bgr_min[1], 255, on_change);
	cv::createTrackbar("minR", "������������", &bgr_min[2], 255, on_change);
	cv::createTrackbar("maxB", "������������", &bgr_max[0], 255, on_change);
	cv::createTrackbar("maxG", "������������", &bgr_max[1], 255, on_change);
	cv::createTrackbar("maxR", "������������", &bgr_max[2], 255, on_change);
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
	//˫���˲�
	bilateralFilter(src, dst, 25, 1000, 1000);
	//������ֵ��
	inRange(dst, cv::Scalar(bgr_min[0], bgr_min[1], bgr_min[2]), cv::Scalar(bgr_max[0], bgr_max[1], bgr_max[2]), dst);
	//����
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));
	dilate(dst, dst, element);
}

void findball::draw()
{
	//����������ɸѡ����
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	std::vector<cv::Rect> rects;

	cv::findContours(dst, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);	//��������

	for (int i = 0; i < contours.size(); i++)										//���벢ɸѡ����
	{
		rects.push_back(cv::boundingRect(contours[i]));
		//ע������û��ʹ����ת����		RotatedRect		minAreaRect()
		if ((rects.back().height < 100 || rects.back().width < 100) ||
			fabsf(rects.back().height - rects.back().width) > 20)
			rects.erase(rects.end() - 1);
	}

	//�ҳ�Բ�Ĳ��һ���Բ
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
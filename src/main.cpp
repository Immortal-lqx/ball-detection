#include <opencv2/opencv.hpp>
#include "findball.h"

cv::VideoCapture findball::capture(0);

int main()
{
	findball found;
	
	while (1)
	{
		found.getpicture();
		found.process();
		found.draw();
		found.show();
	}

	return 0;
}
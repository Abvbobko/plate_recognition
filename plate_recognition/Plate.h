#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

struct Plate
{
	Plate(vector<cv::Point> &plateCoordinates);

	cv::Point minPoint; // left top
	cv::Point maxPoint; // right bottom

	int height;
	int width;

	bool operator < (const Plate &otherPlate);// const;
	bool operator == (const Plate &otherPlate);// const;
};


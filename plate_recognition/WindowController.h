#pragma once

// GDI libs
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

// opencv
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <algorithm>

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <stdexcept>
using namespace cv;

#define MAIN_IMG_X       10
#define MAIN_IMG_Y       10
#define MAIN_IMG_WIDTH   600
#define MAIN_IMG_HEIGHT  400

/*
* WindowController - class for window events managment.
*/

class WindowController
{
public:
	void DrawImageRect(HDC hdc, Mat picture);
};


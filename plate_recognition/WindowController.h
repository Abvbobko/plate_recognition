#pragma once

#define _CRT_SECURE_NO_WARNINGS

// for strings
#include <locale>
using namespace std;

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

// main rect and image size
#define MAIN_IMG_X       10
#define MAIN_IMG_Y       10
#define MAIN_IMG_WIDTH   600
#define MAIN_IMG_HEIGHT  400

// text inside main rect
#define MAIN_RECT_TEXT    L"No images uploaded."
#define FONT_FAMILY       L"Helvetica"
#define FONT_COLOR        Color::Black
#define FONT_SIZE         12
#define FONT_STYLE        FontStyleBold
#define TEXT_START_POS_X  (MAIN_IMG_WIDTH)/2.5
#define TEXT_START_POS_Y  (MAIN_IMG_HEIGHT - FONT_SIZE)/2


/*
* WindowController - class for window events managment.
*/
class WindowController
{
public:
	void DrawImageRect(HDC hdc, Mat picture);
};


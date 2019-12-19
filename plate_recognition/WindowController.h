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

#include "RecognitionTools.h"
#include "plate_recognition.h"

// font info
#define FONT_FAMILY  L"Helvetica"
#define FONT_COLOR   Color::Black
#define FONT_SIZE    12

class WindowController
{
public:
	void DrawComponents(HDC hdc);	
	int GetPagesNum();
	void IncPage();
	void DecPage();
	bool IsPagesStart();
	bool IsPagesEnd();
	void SavePlate(char * filePath);
	void SaveNPlate(char * filePath);
	void SetImage(char * filePath);
	bool Recognize();
	WindowController();
	~WindowController();
private:
	void DrawImageRect(HDC hdc, int rectL, int rectT,
		int rectR, int rectB, const WCHAR * text, int textX, int textY);
	
	int page = 0;
	RecognitionTools * recTools;
};


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
	void DrawComponents(HDC hdc);    // draw images on window
	int GetError() const;            // get error info
	const WCHAR * GetErrorText(int errorCode); 
	int GetPagesNum() const; 
	void IncPage();                  // to next page
	void DecPage();                  // to prev page
	bool IsPagesStart() const;       // is the first page?
	bool IsPagesEnd() const;         // is the last page?
	void SavePlate(char * filePath); // save plate to file
	void SaveNPlate(char * filePath);
	void SetImage(char * filePath); 
	bool Recognize();                // recognize license plates
	WindowController();
	~WindowController();
private:
	// draw one rect + text inside
	void DrawImageRect(HDC hdc, int rectL, int rectT,
		int rectR, int rectB, const WCHAR * text, int textX, int textY); 
	int error;
	int page = 0;	
	RecognitionTools * recTools; // tools for recognition
};


#pragma once

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
using namespace std;

#include "Plate.h"

#define PLATE_CASCADE_PATH "resources\\plate_cascade.xml"//"C:\\Users\\hp\\Desktop\\course_work\\plate_cascade.xml"
#define SYMBOL_CASCADE_PATH "resources\\symbol_cascade.xml"//"C:\\Users\\hp\\Desktop\\course_work\\symbol_cascade.xml"
/*
* Class for licanse plate recognision
*/

class RecognitionTools
{
public:
	//bool Recognize();
	//bool Recognize(Mat &img);
	void SetImage(Mat &img);
	Mat GetImage() const;
	//void SaveLicensePlates();
	//void ShowLicensePlates();
	//vector<Mat> getLicensePlates();
	//vector<string> getLicenseText() const;
	//vector<Mat> getLicensePlates() const;

	RecognitionTools();
	~RecognitionTools();
private:
//	const int scale = 2;
//	const unsigned thresh = 160;
//	const double minDegree = -10;
//	const double maxDegree = 10;
//	const double stepDegree = 0.1;

	Mat carPicture;
	
	//bool findLetters(Mat& src);
	//double getAngle(Mat& plate);
	//unsigned getBottomBound(cv::Mat& plate);
	//unsigned getTopBound(cv::Mat& plate);
	//unsigned getHistTopBound(cv::Mat& plate);
	
	// обьединить  left и right 
	//unsigned getRightBound(cv::Mat plate, bool iswhite);
	//unsigned getLeftBound(cv::Mat plate, bool iswhite);
	
	//bool recognizeLetters();
	
	//void rotateImage(cv::Mat& image, const double angle);
	//vector<PlateArea> plateSymbols;
	//vector<string>	plateText;
	vector<Mat>	licensePlates;

	//const std::string symbolDigit = "0123456789";
	//const std::string symbolChar = "abekmhopctyxABEKMHOPCTYX";

	//tesseract::TessBaseAPI OCR;
	CascadeClassifier plateCascadeClassifier;
	CascadeClassifier symbolCascadeClassifier;
};


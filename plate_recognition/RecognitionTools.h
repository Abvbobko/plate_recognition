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

#include "plate_rec_constants.h"
using namespace cv;
using namespace std;

#define PLATE_CASCADE_PATH   "resources\\plate_cascade.xml"
#define SYMBOL_CASCADE_PATH  "resources\\symbol_cascade.xml"

#define SCALE_FACTOR_PLATES_CL       1.1
#define MIN_NEIGHBORS_NUM_PLATES_CL  10

#define MIN_PLATE_RECT_SIZE  Size(3, 3)
#define MAX_PLATE_RECT_SIZE  Size(500, 150)

#define HALF_WIDTH   480 
#define HALF_HEIGHT  320

#define MIN_ANGLE   -10
#define MAX_ANGLE   10
#define STEP_ANGLE  0.1

#define SCALE 2


class RecognitionTools
{
public:	
	void ClearOutput();
	int GetError();
	Mat GetImage() const;
	vector<Mat> GetLicensePlates() const;
	vector<Mat> GetNormalizedPlates() const;
	bool Recognize();		
	void ResetError();
	void SetImage(Mat &img);
	RecognitionTools();
	~RecognitionTools();
private:
	Mat Normalize(Mat &src);
	double GetAngle(Mat &plate);
	int GetBottomBound(Mat &plate);
	int GetTopBound(Mat &plate);
	int GetHistTopBound(Mat &plate);
	int GetRightBound(Mat plate, bool iswhite);
	int GetLeftBound(Mat plate, bool iswhite);
	void RotateImage(Mat &image, const double angle);	

	Mat carPicture;
	int error = NO_ERRORS;
	vector<Mat>	licensePlates;
	vector<Mat>	normalizedPlates;

	CascadeClassifier plateCascadeClassifier;
	CascadeClassifier symbolCascadeClassifier;
};


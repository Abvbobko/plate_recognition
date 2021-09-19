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

// path of file with models weights	
#define PLATE_CASCADE_PATH   "resources\\plate_cascade.xml"
#define SYMBOL_CASCADE_PATH  "resources\\symbol_cascade.xml"

// parameters for model
#define SCALE_FACTOR_PLATES_CL       1.1
#define MIN_NEIGHBORS_NUM_PLATES_CL  10

// sizes of window (max and min possible size of plate)
#define MIN_PLATE_RECT_SIZE  Size(3, 3)
#define MAX_PLATE_RECT_SIZE  Size(500, 150)

#define HALF_WIDTH   480 
#define HALF_HEIGHT  320

// angle
#define MIN_ANGLE   -10
#define MAX_ANGLE   10
#define STEP_ANGLE  0.1

#define SCALE 2


class RecognitionTools
{
public:	
	void ClearOutput(); // clear old images
	int GetError() const; 
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
	int GetBottomBound(Mat &plate); // get bottom of plate
	int GetTopBound(Mat &plate);    // get top of plate
	int GetHistTopBound(Mat &plate); 
	int GetRightBound(Mat plate, bool iswhite); // get right of plate
	int GetLeftBound(Mat plate, bool iswhite);  // get left of plate
	void RotateImage(Mat &image, const double angle);

	Mat carPicture;
	int error = NO_ERRORS;
	vector<Mat>	licensePlates;
	vector<Mat>	normalizedPlates;

	CascadeClassifier plateCascadeClassifier;
	CascadeClassifier symbolCascadeClassifier;
};


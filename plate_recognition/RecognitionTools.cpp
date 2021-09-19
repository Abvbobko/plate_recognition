#include "RecognitionTools.h"

RecognitionTools::RecognitionTools()
{
	// load plate Haar cascade classifier
	if (!plateCascadeClassifier.load(PLATE_CASCADE_PATH)) {		
		error = PLATE_CLASSIFIER_LOAD_ERROR;
	}

	// load symbol Haar cascade classifier
	if (!symbolCascadeClassifier.load(SYMBOL_CASCADE_PATH)) {
		error = SYMBOL_CLASSIFIER_LOAD_ERROR;
	}
}

RecognitionTools::~RecognitionTools()
{
	for (auto &plate : licensePlates) {
		plate.release();
	}
	carPicture.release();
}

bool RecognitionTools::Recognize()
{
	// recognize license plates from image and normalized it
	ResetError();
	// if we haven't image - return error
	if (carPicture.empty()) {
		error = NO_LOADED_IMAGE;
		return false;
	}

	ClearOutput();

	Mat grayPicture;
	// convert BGR image to gray omage
	cvtColor(carPicture, grayPicture, COLOR_BGR2GRAY);
	// CV_8U is unsigned 8bit/pixel - ie a pixel can have values 0-255
	grayPicture.convertTo(grayPicture, CV_8U);

	// if image too big
	bool needResize = (carPicture.size().width / SCALE > HALF_WIDTH)
		|| (carPicture.size().height / SCALE > HALF_HEIGHT);

	if (needResize) {
		// resize big image
		// INTER_LINEAR - bilinear interpolation
		resize(grayPicture, grayPicture, 
			Size(carPicture.size().width / SCALE,
				grayPicture.size().height / SCALE),
			0, 0, INTER_LINEAR);
	}

	vector<Rect> plates;
	// get plates from image
	plateCascadeClassifier.detectMultiScale(grayPicture, plates, SCALE_FACTOR_PLATES_CL, MIN_NEIGHBORS_NUM_PLATES_CL, 0);

	// save licanse plates
	for (auto &point : plates) {
		Point plateBegin;
		Point plateEnd;

		int coef = (needResize) ? SCALE : 1;
		plateBegin = Point(point.x * coef, point.y * coef);
		plateEnd = Point(point.width * coef, point.height * coef);
		
		licensePlates.push_back(carPicture(Rect(plateBegin.x, plateBegin.y, plateEnd.x, plateEnd.y)));
	}

	if (licensePlates.empty()) {
		error = NO_RECOGNIZED_ELEMENTS_ERROR;
		return false;
	}

	// save normalized license plates
	for (auto &p : licensePlates) {
		normalizedPlates.push_back(Normalize(p));
	}
	return true;
}

void RecognitionTools::ResetError()
{
	// reset old error
	error = NO_ERRORS;
}

void RecognitionTools::SetImage(Mat &img)
{
	// set image for operation with it
	this->carPicture = img;
}

void RecognitionTools::ClearOutput()
{
	// clear old output
	normalizedPlates.clear();
	licensePlates.clear();
}

int RecognitionTools::GetError() const
{
	// get error code
	return error;
}

Mat RecognitionTools::GetImage() const
{	
	return this->carPicture;
}

vector<Mat> RecognitionTools::GetLicensePlates() const
{
	return this->licensePlates;
}

vector<Mat> RecognitionTools::GetNormalizedPlates() const
{
	return this->normalizedPlates;
}

Mat RecognitionTools::Normalize(Mat &src)
{	
	cv::Mat srcGray, srcThreshold;
	// convert image from RGB to Gray
	cvtColor(src, srcGray, COLOR_BGR2GRAY);
	// Treshold - For every pixel, the same threshold value is applied. 
	// If the pixel value is smaller than the threshold, it is set to 0, otherwise it is set to a maximum value.
	// THRESH_BINARY - func for tresholding
	// THRESH_OTSU - flag, use Otsu algorithm to choose the optimal threshold value
	threshold(srcGray, srcThreshold, 0, 255, THRESH_BINARY | THRESH_OTSU);
	
	// Blurs an image using the median filter.
	// ksize - aperture linear size
	medianBlur(srcThreshold, srcThreshold, 5);

	// get angle at which number is rotated
	double angle = GetAngle(srcThreshold);
	// rotate number to normal angle
	RotateImage(srcThreshold, angle);

	// get bounds of plate
	int bottomBound = GetBottomBound(srcThreshold);
	int topBound = GetTopBound(srcThreshold);

	srcThreshold = srcThreshold(Rect(0, topBound, srcThreshold.size().width, bottomBound - topBound));
	
	// get bounds for image if car is white and of not. Return best result
	int leftBound = max(GetLeftBound(srcThreshold, true), GetLeftBound(srcThreshold, false));
	int rightBound = min(GetRightBound(srcThreshold, true), GetRightBound(srcThreshold, false));
	
	// normalize image according to the obtained parameters
	Mat normalizedImg = src.clone();
	RotateImage(normalizedImg, angle);
	return normalizedImg(Rect(leftBound, topBound, rightBound - leftBound, bottomBound - topBound));
}

double RecognitionTools::GetAngle(Mat &plate)
{
	// get angle at which number is rotated
	int min = plate.size().height;
	double resultAngle = 0;
	Mat tmp;

	// rotate image on different angles, return angle with the best bottom bound
	for (double angle = MIN_ANGLE; angle < MAX_ANGLE; angle += STEP_ANGLE) {
		tmp = plate.clone();
		RotateImage(tmp, angle);
		int bottomBound = GetBottomBound(tmp);
		if (bottomBound < min) {
			resultAngle = angle;
			min = bottomBound;
		}
	}
	return resultAngle;
}

int RecognitionTools::GetBottomBound(Mat &plate)
{
	size_t height = plate.size().height;
	int lastCount = 0;
	Mat data;

	for (int i = height / 2; i < height; i++) {
		data = plate.row(i);
		// Counts non-zero array elements.
		// zero == black
		int count = countNonZero(data);
		// if number of black pixels is the largest
		if (count < lastCount / 2) {
			return i;
		}

		lastCount = count;
	}
	return height;
}

int RecognitionTools::GetTopBound(Mat &plate)
{
	vector<Rect> symbols;
	// get symbols from plate
	symbolCascadeClassifier.detectMultiScale(plate, symbols);

	// calculate average value of height of symbols
	int averageHeight = 0;
	for (auto &s : symbols) {
		averageHeight += s.y;
	}

	if (!symbols.empty()) {
		averageHeight /= symbols.size();
	}
	// if we can't recognized symbols - get bound from hist
	return symbols.empty() ? GetHistTopBound(plate) : averageHeight;
}

int RecognitionTools::GetHistTopBound(Mat &plate)
{
	size_t height = plate.size().height;
	Mat data;

	for (int i = 0; i < height / 2; i++) {
		data = plate.row(i);
		// Counts non-zero array elements.
		// zero == black
		int count = cv::countNonZero(data);

		if (count > height/2) {
			return i;
		}
	}
	return 0;
}

int RecognitionTools::GetRightBound(Mat plate, bool iswhite)
{
	// getStructuringElement - Returns a structuring element 
	// of the specified size and shape for morphological operations.
	// MORPH_RECT - a rectangular structuring element
	Mat element = getStructuringElement(MORPH_RECT,
		Size(3, 3),
		Point(1, 1)
	);
	// erode - erodes an image by using a specific structuring element.
	// dilate - dilates an image by using a specific structuring element.
	cv::erode(plate, plate, element);
	cv::dilate(plate, plate, element);

	size_t width = plate.size().width;
	double height = plate.size().height;

	Mat data;

	for (int i = width - 2; i > width / 2; i--) {
		data = plate.col(i);
		int count = countNonZero(data);

		if ((!iswhite && count > height*0.5) || (iswhite && count < height*0.60)) {
			return i + 1;
		}
	}

	return width;
}

int RecognitionTools::GetLeftBound(Mat plate, bool iswhite)
{
	Mat element = getStructuringElement(MORPH_RECT,
		Size(3, 3),
		Point(1, 1));
	cv::erode(plate, plate, element);
	cv::dilate(plate, plate, element);

	size_t width = plate.size().width;
	double height = plate.size().height;

	Mat data;

	for (int i = 2; i < width / 2; i++) {
		data = plate.col(i);
		int count = countNonZero(data);

		if ((!iswhite && count > height*0.5) || (iswhite && count < height*0.60)) {
			return i;
		}
	}
	return 0;
}

void RecognitionTools::RotateImage(Mat &image, const double angle)
{
	// CV_32FC1 is 32 bit floating point signed depth in one channel
	Mat rot_mat(2, 3, CV_32FC1);

	Point center = Point(image.cols / 2, image.rows / 2);
	double scale = 1;

	// Calculates an affine matrix of 2D rotation.
	rot_mat = getRotationMatrix2D(center, angle, scale);
	// Applies an affine transformation to an image.
	warpAffine(image, image, rot_mat, image.size());
}

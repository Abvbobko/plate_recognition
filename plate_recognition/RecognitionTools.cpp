#include "RecognitionTools.h"

RecognitionTools::RecognitionTools()
{
	if (!plateCascadeClassifier.load(PLATE_CASCADE_PATH)) {		
		// Plate cascade load error
	}

	if (!symbolCascadeClassifier.load(SYMBOL_CASCADE_PATH)) {
		// Symbol cascade load error		
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
	if (carPicture.empty()) {
		// No image for recognize error
		return false;
	}

	normalizedPlates.clear();
	licensePlates.clear();	

	Mat grayPicture;
	cvtColor(carPicture, grayPicture, COLOR_BGR2GRAY);
	grayPicture.convertTo(grayPicture, CV_8U);

	bool needResize = (carPicture.size().width / SCALE > HALF_WIDTH)
		|| (carPicture.size().height / SCALE > HALF_HEIGHT);

	if (needResize) {
		resize(grayPicture, grayPicture, 
			Size(carPicture.size().width / SCALE,
				grayPicture.size().height / SCALE),
			0, 0, INTER_LINEAR);
	}

	vector<Rect> plates;
	plateCascadeClassifier.detectMultiScale(grayPicture, plates, SCALE_FACTOR_PLATES_CL, MIN_NEIGHBORS_NUM_PLATES_CL, 0);

	for (auto &point : plates) {
		Point plateBegin;
		Point plateEnd;

		int coef = (needResize) ? SCALE : 1;
		plateBegin = Point(point.x * coef, point.y * coef);
		plateEnd = Point(point.width * coef, point.height * coef);
		
		licensePlates.push_back(carPicture(Rect(plateBegin.x, plateBegin.y, plateEnd.x, plateEnd.y)));
	}

	for (auto &p : licensePlates) {
		normalizedPlates.push_back(Normalize(p));
	}
	return true;
}

void RecognitionTools::SetImage(Mat &img)
{
	this->carPicture = img;
}

Mat RecognitionTools::GetImage() const
{
	return this->carPicture;
}

void RecognitionTools::SaveLicensePlates()
{
	// изменить дл€ виндоус и диалогов. ћќжно даже не тут сохран€ть а в контроллере
	size_t id = 0;
	for (auto &p : licensePlates) {
		string name("license plate " + to_string(id) + ".jpg");
		imwrite(name, p);
		id++;
	}
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

	cvtColor(src, srcGray, COLOR_BGR2GRAY);
	threshold(srcGray, srcThreshold, 0, 255, THRESH_BINARY | THRESH_OTSU);
	medianBlur(srcThreshold, srcThreshold, 5);

	double angle = GetAngle(srcThreshold);
	RotateImage(srcThreshold, angle);

	int bottomBound = GetBottomBound(srcThreshold);
	int topBound = GetTopBound(srcThreshold);

	srcThreshold = srcThreshold(Rect(0, topBound, srcThreshold.size().width, bottomBound - topBound));
	
	int leftBound = max(GetLeftBound(srcThreshold, true), GetLeftBound(srcThreshold, false));
	int rightBound = min(GetRightBound(srcThreshold, true), GetRightBound(srcThreshold, false));
	
	Mat normalizedImg = src.clone();
	RotateImage(normalizedImg, angle);
	return normalizedImg(Rect(leftBound, topBound, rightBound - leftBound, bottomBound - topBound));
}

double RecognitionTools::GetAngle(Mat &plate)
{
	int min = plate.size().height;
	double resultAngle = 0;
	Mat tmp;

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
		int count = countNonZero(data);

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

	symbolCascadeClassifier.detectMultiScale(plate, symbols);

	int averageHeight = 0;
	for (auto &s : symbols) {
		averageHeight += s.y;
	}

	if (!symbols.empty()) {
		averageHeight /= symbols.size();
	}

	return symbols.empty() ? GetHistTopBound(plate) : averageHeight;
}

int RecognitionTools::GetHistTopBound(Mat &plate)
{
	size_t height = plate.size().height;
	Mat data;

	for (int i = 0; i < height / 2; i++) {
		data = plate.row(i);
		int count = cv::countNonZero(data);

		if (count > height*0.5) {
			return i;
		}
	}

	return 0;
}

int RecognitionTools::GetRightBound(Mat plate, bool iswhite)
{
	Mat element = getStructuringElement(MORPH_RECT,
		cv::Size(3, 3),
		cv::Point(1, 1));
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
		cv::Size(3, 3),
		cv::Point(1, 1));
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
	Mat rot_mat(2, 3, CV_32FC1);

	cv::Point center = cv::Point(image.cols / 2, image.rows / 2);
	double scale = 1;

	rot_mat = getRotationMatrix2D(center, angle, scale);
	warpAffine(image, image, rot_mat, image.size());
}

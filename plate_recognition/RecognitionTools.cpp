#include "RecognitionTools.h"

RecognitionTools::RecognitionTools()
{
	if (!plateCascadeClassifier.load(PLATE_CASCADE_PATH)) {
		
		//std::cout << "Plate Cascade Load Error";
	}

	if (!symbolCascadeClassifier.load(SYMBOL_CASCADE_PATH)) {
		
		//std::cout << "Symbol Cascade Load Error";
	}

	//	OCR.Init(NULL, "C:\\Users\\hp\\Desktop\\course_work\\tessdata\\amh.traineddata");
		//OCR.SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
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
		throw std::logic_error("Images for recognize is empty.");////////////////////////////////
	}

	licensePlates.clear();
	plateSymbols.clear();
	plateText.clear();

	Mat grayPicture;

	cvtColor(carPicture, grayPicture, COLOR_BGR2GRAY);
	grayPicture.convertTo(grayPicture, CV_8U);
	bool needResize = (carPicture.size().width / scale > HALF_WIDTH) || (carPicture.size().height / scale > HALF_HEIGHT);

	if (needResize) {
		resize(grayPicture, grayPicture, 
			Size(carPicture.size().width / scale, 
				grayPicture.size().height / scale), 
			0, 0, INTER_LINEAR);
	}
	vector<Rect> plates;
	//equalizeHist(grayPicture, grayPicture);

	plateCascadeClassifier.detectMultiScale(grayPicture, plates, SCALE_FACTOR_PLATES_CL, MIN_NEIGHBORS_NUM_PLATES_CL, 0);

	for (auto &point : plates)
	{
		Point plateBegin;
		Point plateEnd;

		// Сделать просто тернарный оператор
		if (needResize) {
			plateBegin = Point(point.x * scale, point.y * scale);
			plateEnd = Point(point.width * scale, point.height * scale);
		}
		else {
			plateBegin = Point(point.x, point.y);
			plateEnd = Point(point.width, point.height);
		}

		licensePlates.push_back(carPicture(Rect(plateBegin.x, plateBegin.y, plateEnd.x, plateEnd.y)));
	}

	for (auto& p : licensePlates) {
		findLetters(p);
	}

	//if (!plateSymbols.empty())
	//	recognizeLetters();

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
	size_t id = 0;
	for (auto &p : licensePlates)
	{
		string name("license plate " + to_string(id) + ".jpg");
		imwrite(name, p);
		id++;
	}
}

vector<Mat> RecognitionTools::getLicensePlates()
{
	return this->licensePlates;
}

vector<string> RecognitionTools::getLicenseText() const
{
	return this->plateText;
}

vector<Mat> RecognitionTools::getLicensePlates() const
{
	return this->licensePlates;
}

PlateArea::PlateArea(Mat & plate, vector<Plate>& plateAreaSymbols)
{
	this->plate = plate;
	this->plateAreaSymbols = plateAreaSymbols;
}

bool RecognitionTools::findLetters(Mat & src) //Rename to print number in black-white + delete big trashhold
{
	vector<vector<cv::Point> > contours;
	vector<Plate> contursOut;
	vector<Vec4i> hierarchy;
	cv::Mat cannyOutput, srcGray, srcThreshold;

	cvtColor(src, srcGray, COLOR_BGR2GRAY);
	threshold(srcGray, srcThreshold, 0, 255, THRESH_BINARY | THRESH_OTSU);
	medianBlur(srcThreshold, srcThreshold, 5);

	double angle = getAngle(srcThreshold);
	rotateImage(srcThreshold, angle);

	unsigned bottomBound = getBottomBound(srcThreshold);
	unsigned topBound = getTopBound(srcThreshold);
	srcThreshold = srcThreshold(cv::Rect(0, topBound, srcThreshold.size().width, bottomBound - topBound));
	unsigned leftBound = std::max(getLeftBound(srcThreshold, true), getLeftBound(srcThreshold, false));
	unsigned rightBound = std::min(getRightBound(srcThreshold, true), getRightBound(srcThreshold, false));
	///////////////////////////////

	srcThreshold = srcThreshold(cv::Rect(leftBound, 0, rightBound - leftBound, srcThreshold.size().height));
//	cv::namedWindow("numbers", cv::WINDOW_AUTOSIZE);

	//cv::imshow("numbers", srcThreshold); // Это надо будет вернуть и вывести
	return false;
}

double RecognitionTools::getAngle(Mat & plate)
{
	int min = plate.size().height;
	double angle = 0;
	cv::Mat temp;

	for (double a = minDegree; a < maxDegree; a += stepDegree) //a - angle 
	{
		temp = plate.clone();
		rotateImage(temp, a);

		unsigned bottomBound = getBottomBound(temp);
		if (bottomBound < min)
		{
			angle = a;
			min = bottomBound;
		}
	}

	return angle;
}

unsigned RecognitionTools::getBottomBound(cv::Mat & plate)
{
	size_t height = plate.size().height;
	unsigned lastCount = 0;
	cv::Mat data;

	for (unsigned i = height / 2; i < height; i++) {
		data = plate.row(i);
		unsigned count = cv::countNonZero(data);

		if (count < lastCount / 2)
			return i;

		lastCount = count;
	}

	return height;
}

unsigned RecognitionTools::getTopBound(cv::Mat & plate)
{
	std::vector<cv::Rect> symbols;

	symbolCascadeClassifier.detectMultiScale(plate, symbols);

//	if (symbols.empty())
		//std::cout << "Symbols not found" << std::endl;

	//std::vector<cv::Rect>::iterator result;

	//std::sort(symbols.begin(), symbols.end(), [](const cv::Rect& r1, const cv::Rect& r2) {return r1.y > r2.y;});

	/* вывод картинок всех букв!!!!!!!!!!
	int i = 0;
	for (auto& s : symbols) {
		cv::namedWindow(to_string(i), WINDOW_AUTOSIZE);
		cv::imshow(to_string(i), plate(s));
		i++;
	}*/

	//if (showInfo)
		//for (auto& s : symbols)
		//	std::cout << s.y << std::endl;

	unsigned averageHeight = 0;
	for (auto& s : symbols)
		averageHeight += s.y;

	if (!symbols.empty()) // is zero
		averageHeight /= symbols.size();

	return symbols.empty() ? getHistTopBound(plate) : averageHeight;
}

unsigned RecognitionTools::getHistTopBound(cv::Mat & plate)
{
	size_t height = plate.size().height;
	cv::Mat data;

	for (unsigned i = 0; i < height / 2; ++i)
	{
		data = plate.row(i);
		unsigned count = cv::countNonZero(data);

		if (count > height*0.5)
			return i;
	}

	return 0;
}

unsigned RecognitionTools::getRightBound(cv::Mat plate, bool iswhite)
{
	cv::Mat element = getStructuringElement(cv::MORPH_RECT,
		cv::Size(2 * 1 + 1, 2 * 1 + 1),
		cv::Point(1, 1));
	cv::erode(plate, plate, element);
	cv::dilate(plate, plate, element);

	size_t width = plate.size().width;
	double height = plate.size().height;

	cv::Mat data;

	for (unsigned i = width - 2; i > width / 2; --i)
	{
		data = plate.col(i);
		unsigned count = cv::countNonZero(data);

		if ((!iswhite && count > height*0.5) || (iswhite && count < height*0.60))
			return i + 1;
	}

	return width;
}

unsigned RecognitionTools::getLeftBound(cv::Mat plate, bool iswhite)
{
	cv::Mat element = getStructuringElement(cv::MORPH_RECT,
		cv::Size(2 * 1 + 1, 2 * 1 + 1),
		cv::Point(1, 1));
	cv::erode(plate, plate, element);
	cv::dilate(plate, plate, element);

	size_t width = plate.size().width;
	double height = plate.size().height;

	cv::Mat data;

	for (unsigned i = 2; i < width / 2; ++i)
	{
		data = plate.col(i);
		unsigned count = cv::countNonZero(data);

		if ((!iswhite && count > height*0.5) || (iswhite && count < height*0.60))
			return i;
	}

	return 0;
}

void RecognitionTools::rotateImage(cv::Mat & image, const double angle)
{
	cv::Mat rot_mat(2, 3, CV_32FC1);

	cv::Point center = cv::Point(image.cols / 2, image.rows / 2);
	double scale = 1;

	rot_mat = getRotationMatrix2D(center, angle, scale);
	warpAffine(image, image, rot_mat, image.size());
}

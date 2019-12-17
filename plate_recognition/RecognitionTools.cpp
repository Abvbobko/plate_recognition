#include "RecognitionTools.h"

RecognitionTools::RecognitionTools()
{
	if (!plateCascadeClassifier.load(PLATE_CASCADE_PATH)) {
		exit;
		//std::cout << "Plate Cascade Load Error";
	}

	if (!symbolCascadeClassifier.load(SYMBOL_CASCADE_PATH)) {
		exit;
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

void RecognitionTools::SetImage(Mat &img)
{
	this->carPicture = img;
}

Mat RecognitionTools::GetImage() const
{
	return this->carPicture;
}

#include "WindowController.h"

void WindowController::DrawComponents(HDC hdc)
{
	// draw images and rect on window
	// main rect for image
	DrawImageRect(hdc, MAIN_IMG_LEFT, MAIN_IMG_TOP, MAIN_IMG_RIGHT, MAIN_IMG_BOTTOM,
		MAIN_RECT_TEXT, MAIN_TEXT_START_POS_X, MAIN_TEXT_START_POS_Y);

	// rect for license plate
	DrawImageRect(hdc, PLATE_IMG_LEFT, PLATE_IMG_TOP, PLATE_IMG_RIGHT, PLATE_IMG_BOTTOM,
		PLATE_RECT_TEXT, PLATE_TEXT_START_POS_X, PLATE_TEXT_START_POS_Y);

	// rect for normalized license plate
	DrawImageRect(hdc, NORM_IMG_LEFT, NORM_IMG_TOP, NORM_IMG_RIGHT, NORM_IMG_BOTTOM,
		NORM_RECT_TEXT, NORM_TEXT_START_POS_X, NORM_TEXT_START_POS_Y);

	// if we have image - draw it
	Mat img = recTools->GetImage();
	if (img.data) {
		Graphics gr(hdc);
		// rect coordinates
		PointF points[3] = {
			PointF(MAIN_IMG_LEFT, MAIN_IMG_TOP),
			PointF(MAIN_IMG_RIGHT, MAIN_IMG_TOP),
			PointF(MAIN_IMG_LEFT, MAIN_IMG_BOTTOM)
		};
		// convert opencv Mat to GDI+ bitmap for drawing
		cv::Size size = img.size();
		Bitmap bitmap(size.width, size.height, img.step1(), PixelFormat24bppRGB, img.data);
		gr.DrawImage(&bitmap, points, 3);
	}
	
	// if we have license plate - draw it
	vector<Mat> licensePlates = recTools->GetLicensePlates();
	if (!licensePlates.empty()) {
		Mat plate = licensePlates[page];
		if (plate.data) {
			Graphics gr(hdc);
			PointF points[3] = {
				PointF(PLATE_IMG_LEFT, PLATE_IMG_TOP),
				PointF(PLATE_IMG_RIGHT, PLATE_IMG_TOP),
				PointF(PLATE_IMG_LEFT, PLATE_IMG_BOTTOM)
			};
			// convert opencv Mat to GDI+ bitmap for drawing
			cv::Size size = plate.size();
			Bitmap bitmap(size.width, size.height, plate.step1(), PixelFormat24bppRGB, plate.data);
			gr.DrawImage(&bitmap, points, 3);
		}
	}	
	// if we have normalized license plate - draw it
	vector<Mat> normalizedPlates = recTools->GetNormalizedPlates();
	if (!normalizedPlates.empty()) {
		Mat plate = normalizedPlates[page];		
		if (plate.data) {
			Graphics gr(hdc);
			PointF points[3] = {
				PointF(NORM_IMG_LEFT, NORM_IMG_TOP),
				PointF(NORM_IMG_RIGHT, NORM_IMG_TOP),
				PointF(NORM_IMG_LEFT, NORM_IMG_BOTTOM)
			};
			// convert opencv Mat to GDI+ bitmap for drawing
			cv::Size size = plate.size();			
			Bitmap bitmap(size.width, size.height, plate.step1(), PixelFormat24bppRGB, plate.data);
			gr.DrawImage(&bitmap, points, 3);
		}
	}
}

int WindowController::GetError() const
{
	// return last error code
	return error;
}

const WCHAR * WindowController::GetErrorText(int errorCode)
{	
	// return text of last error and reset error code
	error = NO_ERRORS;
	switch (errorCode)
	{
	case NO_ERRORS:
		return NO_ERRORS_TEXT;

	case PLATE_CLASSIFIER_LOAD_ERROR:
		return PLATE_CLASSIFIER_LOAD_ERROR_TEXT;

	case SYMBOL_CLASSIFIER_LOAD_ERROR:
		return SYMBOL_CLASSIFIER_LOAD_ERROR_TEXT;

	case SAVE_PLATE_ERROR:
		return SAVE_PLATE_ERROR_TEXT;

	case SAVE_NPLATE_ERROR:
		return SAVE_NPLATE_ERROR_TEXT;

	case FILE_OPEN_ERROR:
		return FILE_OPEN_ERROR_TEXT;

	case NO_LOADED_IMAGE:
		return NO_LOADED_IMAGE_TEXT;

	case NO_RECOGNIZED_ELEMENTS_ERROR:
		return NO_RECOGNIZED_ELEMENTS_ERROR_TEXT;

	case NO_PLATES_ERROR:
		return NO_PLATES_ERROR_TEXT;

	case NO_NPLATES_ERROR:
		return NO_NPLATES_ERROR_TEXT;
	default:
		return UNKNOWN_ERROR_TEXT;		
	}
}

int WindowController::GetPagesNum() const
{
	// get num of recognized license plates
	return recTools->GetLicensePlates().size();
}

void WindowController::IncPage()
{
	// go to next page
	if (page + 1 < GetPagesNum()) {
		page++;
	}
}

void WindowController::DecPage()
{
	// go to prev page
	if (page - 1 >= 0) {
		page--;
	}
}

bool WindowController::IsPagesStart() const
{
	// is the first page?
	return page == 0;
}

bool WindowController::IsPagesEnd() const
{
	// is the last page?
	return page == GetPagesNum() - 1;
}

void WindowController::SavePlate(char * filePath)
{
	// save image of current license plate (from carrunt page)
	try {
		if (!recTools->GetLicensePlates().empty()) {
			// save image
			imwrite(filePath, recTools->GetLicensePlates().at(page));
		}
		else {
			error = NO_PLATES_ERROR;
		}
	}
	catch (Exception e){
		error = SAVE_PLATE_ERROR;
	}
}

void WindowController::SaveNPlate(char * filePath)
{
	// save image of current normalized license plate (from carrunt page)
	try {
		if (!recTools->GetNormalizedPlates().empty()) {
			imwrite(filePath, recTools->GetNormalizedPlates().at(page));
		}
		else {
			error = NO_NPLATES_ERROR;
		}
	}
	catch (Exception e) {
		error = SAVE_NPLATE_ERROR;
	}
}

void WindowController::DrawImageRect(HDC hdc, int rectL, int rectT,
	int rectR, int rectB, const WCHAR * text, int textX, int textY)
{	
	// draw rectangle and text for it
	Rectangle(hdc, rectL, rectT, rectR, rectB);
	// draw text inside rect
	Graphics gr(hdc);		
	Font font(&FontFamily(FONT_FAMILY), FONT_SIZE);
	SolidBrush brush(FONT_COLOR);		
	gr.DrawString(text, wcslen(text), &font, PointF(textX, textY), &brush);
}

void WindowController::SetImage(char * filePath)
{
	// set image from which the plates are recognized
	try {
		error = NO_ERRORS;
		// IMREAD_COLOR - convert image to the 3 channel BGR color image
		Mat img = imread(filePath, IMREAD_COLOR);
		page = 0;
		recTools->ClearOutput();
		recTools->SetImage(img);
	}
	catch (Exception e) {
		error = FILE_OPEN_ERROR;
	}

}

bool WindowController::Recognize()
{
	// recognize plates from image
	bool result = recTools->Recognize();
	if (!result) {
		error = NO_RECOGNIZED_ELEMENTS_ERROR;
		page = 0;
	}	
	return result && (recTools->GetLicensePlates().size() > 0);
}

WindowController::WindowController()
{
	recTools = new RecognitionTools;
	error = recTools->GetError();
}

WindowController::~WindowController()
{
	recTools->~RecognitionTools();
}


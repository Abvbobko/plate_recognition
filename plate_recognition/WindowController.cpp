#include "WindowController.h"

void WindowController::DrawComponents(HDC hdc)
{
	Mat img = recTools->GetImage();
	if (img.data) {
		Graphics gr(hdc);
		PointF points[3] = {
			PointF(MAIN_IMG_LEFT, MAIN_IMG_TOP),
			PointF(MAIN_IMG_RIGHT, MAIN_IMG_TOP),
			PointF(MAIN_IMG_LEFT, MAIN_IMG_BOTTOM)
		};
		cv::Size size = img.size();
		Bitmap bitmap(size.width, size.height, img.step1(), PixelFormat24bppRGB, img.data);
		gr.DrawImage(&bitmap, points, 3);
	}
	else {
		DrawImageRect(hdc, MAIN_IMG_LEFT, MAIN_IMG_TOP, MAIN_IMG_RIGHT, MAIN_IMG_BOTTOM,
			MAIN_RECT_TEXT, MAIN_TEXT_START_POS_X, MAIN_TEXT_START_POS_Y);
	}
	vector<Mat> licensePlates = recTools->GetLicensePlates();
	if (!licensePlates.empty()) {
		Mat plate = licensePlates[0];/////////////////
		if (plate.data) {
			Graphics gr(hdc);
			PointF points[3] = {
				PointF(PLATE_IMG_LEFT, PLATE_IMG_TOP),
				PointF(PLATE_IMG_RIGHT, PLATE_IMG_TOP),
				PointF(PLATE_IMG_LEFT, PLATE_IMG_BOTTOM)
			};
			cv::Size size = plate.size();
			Bitmap bitmap(size.width, size.height, plate.step1(), PixelFormat24bppRGB, plate.data);
			gr.DrawImage(&bitmap, points, 3);
		}
	}
	else {
		DrawImageRect(hdc, PLATE_IMG_LEFT, PLATE_IMG_TOP, PLATE_IMG_RIGHT, PLATE_IMG_BOTTOM,
			PLATE_RECT_TEXT, PLATE_TEXT_START_POS_X, PLATE_TEXT_START_POS_Y);
	}

	vector<Mat> normalizedPlates = recTools->GetNormalizedPlates();
	if (!normalizedPlates.empty()) {
		Mat plate = normalizedPlates[0];///////////
		
		if (plate.data) {
			Graphics gr(hdc);
			PointF points[3] = {
				PointF(NORM_IMG_LEFT, NORM_IMG_TOP),
				PointF(NORM_IMG_RIGHT, NORM_IMG_TOP),
				PointF(NORM_IMG_LEFT, NORM_IMG_BOTTOM)
			};
			cv::Size size = plate.size();
			
			Bitmap bitmap(size.width, size.height, plate.step1(), PixelFormat24bppRGB, plate.data);
			gr.DrawImage(&bitmap, points, 3);
		}
	}
	else {
		DrawImageRect(hdc, NORM_IMG_LEFT, NORM_IMG_TOP, NORM_IMG_RIGHT, NORM_IMG_BOTTOM,
			NORM_RECT_TEXT, NORM_TEXT_START_POS_X, NORM_TEXT_START_POS_Y);
	}
}

void WindowController::DrawImageRect(HDC hdc, int rectL, int rectT,
	int rectR, int rectB, const WCHAR * text, int textX, int textY)
{	
	Rectangle(hdc, rectL, rectT, rectR, rectB);

	Graphics gr(hdc);		
	Font font(&FontFamily(FONT_FAMILY), FONT_SIZE);
	SolidBrush brush(FONT_COLOR);		
	gr.DrawString(text, wcslen(text), &font, PointF(textX, textY), &brush);
}

void WindowController::SetImage(char * filePath)
{
	Mat img = imread(filePath, IMREAD_COLOR);
	recTools->SetImage(img);	
}

void WindowController::Recognize()
{
	recTools->Recognize();
}

WindowController::WindowController()
{
	recTools = new RecognitionTools;
}

WindowController::~WindowController()
{
	recTools->~RecognitionTools();
}


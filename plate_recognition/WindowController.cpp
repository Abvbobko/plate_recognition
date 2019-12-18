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
		DrawImageRect(hdc,//, recTools->GetImage(), 
			MAIN_IMG_LEFT, MAIN_IMG_TOP, MAIN_IMG_RIGHT, MAIN_IMG_BOTTOM,
			MAIN_RECT_TEXT, MAIN_TEXT_START_POS_X, MAIN_TEXT_START_POS_Y);
	}
	vector<Mat> licensePlates = recTools->getLicensePlates();
	if (!licensePlates.empty()) {
		Mat a = licensePlates[0];
		if (a.data) {
			Graphics gr(hdc);
			PointF points[3] = {
				PointF(PLATE_IMG_LEFT, PLATE_IMG_TOP),
				PointF(PLATE_IMG_RIGHT, PLATE_IMG_TOP),
				PointF(PLATE_IMG_LEFT, PLATE_IMG_BOTTOM)
			};
			cv::Size size = a.size();
			Bitmap bitmap(size.width, size.height, a.step1(), PixelFormat24bppRGB, a.data);
			gr.DrawImage(&bitmap, points, 3);
		}
	}
	else {
		DrawImageRect(hdc,//, recTools->GetImage(),
			PLATE_IMG_LEFT, PLATE_IMG_TOP, PLATE_IMG_RIGHT, PLATE_IMG_BOTTOM,
			PLATE_RECT_TEXT, PLATE_TEXT_START_POS_X, PLATE_TEXT_START_POS_Y);
	}

	DrawImageRect(hdc,// recTools->GetImage(),
		NORM_IMG_LEFT, NORM_IMG_TOP, NORM_IMG_RIGHT, NORM_IMG_BOTTOM,
		NORM_RECT_TEXT, NORM_TEXT_START_POS_X, NORM_TEXT_START_POS_Y);
	
	
}

void WindowController::DrawImageRect(HDC hdc,/* Mat picture,*/ int rectL, int rectT,
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


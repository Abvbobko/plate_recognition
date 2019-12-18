#include "WindowController.h"

void WindowController::DrawImageRect(HDC hdc, Mat picture)
{
	if (!picture.empty()) {
	}
	else {
		Rectangle(hdc, MAIN_IMG_X, MAIN_IMG_Y, MAIN_IMG_WIDTH, MAIN_IMG_HEIGHT);

		Graphics gr(hdc);		
		Font font(&FontFamily(FONT_FAMILY), FONT_SIZE, FONT_STYLE);
		SolidBrush brush(FONT_COLOR);		
		gr.DrawString(MAIN_RECT_TEXT, wcslen(MAIN_RECT_TEXT), &font,
			PointF(TEXT_START_POS_X, TEXT_START_POS_Y), &brush);
	}
};

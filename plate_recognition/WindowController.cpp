#include "WindowController.h"

void WindowController::DrawImageRect(HDC hdc, Mat picture, int rectL, int rectT, 
	int rectR, int rectB, const WCHAR * text, int textX, int textY)
{
	if (!picture.empty()) {
	}
	else {
		Rectangle(hdc, rectL, rectT, rectR, rectB);

		Graphics gr(hdc);		
		Font font(&FontFamily(FONT_FAMILY), FONT_SIZE);
		SolidBrush brush(FONT_COLOR);		
		gr.DrawString(text, wcslen(text), &font,
			PointF(textX, textY), &brush);
	}
};


#include "WindowController.h"

void WindowController::DrawImageRect(HDC hdc, Mat picture)
{
	if (!picture.empty()) {
	}
	else {
		Rectangle(hdc, MAIN_IMG_X, MAIN_IMG_Y, MAIN_IMG_WIDTH, MAIN_IMG_HEIGHT);

	}
};

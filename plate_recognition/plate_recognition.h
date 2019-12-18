#pragma once

#include "resource.h"

// GDI libs
#include <commdlg.h>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

//program classes
#include "RecognitionTools.h"
#include "WindowController.h"

// CONSTANTS
// window size
#define MIN_WINDOW_WIDTH   850
#define MAX_WINDOW_WIDTH   850
#define MIN_WINDOW_HEIGHT  450
#define MAX_WINDOW_HEIGHT  450

// BUTTONS
// button OPEN
#define B_OPEN_X       605
#define B_OPEN_Y       9
#define B_OPEN_WIDTH   220 
#define B_OPEN_HEIGHT  55
#define B_OPEN_STYLE   WS_VISIBLE | WS_CHILD
#define B_OPEN_TEXT    "Open"
#define B_OPEN_ID      100
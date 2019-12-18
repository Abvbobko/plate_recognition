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

// button RECOGNIZE
#define B_REC_X       605
#define B_REC_Y       5 + B_OPEN_Y + B_OPEN_HEIGHT
#define B_REC_WIDTH   B_OPEN_WIDTH 
#define B_REC_HEIGHT  B_OPEN_HEIGHT
#define B_REC_STYLE   WS_VISIBLE | WS_CHILD
#define B_REC_TEXT    "Recognize"
#define B_REC_ID      200

/*// button SAVE
#define B_SAVE_X       605
#define B_SAVE_Y       5 + B_OPEN_Y + B_OPEN_HEIGHT
#define B_SAVE_WIDTH   B_OPEN_WIDTH 
#define B_SAVE_HEIGHT  B_OPEN_HEIGHT
#define B_SAVE_STYLE   WS_VISIBLE | WS_CHILD
#define B_SAVE_TEXT    "Save"
#define B_SAVE_ID      200*/

// button PREV
#define B_PREV_X       605
#define B_PREV_Y       5 + B_REC_Y + B_REC_HEIGHT
#define B_PREV_WIDTH   B_OPEN_WIDTH/2 - 1 
#define B_PREV_HEIGHT  B_OPEN_HEIGHT
#define B_PREV_STYLE   WS_VISIBLE | WS_CHILD //| WS_DISABLED
#define B_PREV_TEXT    "<"
#define B_PREV_ID      300

// button NEXT
#define B_NEXT_X       605 + B_PREV_WIDTH + 2
#define B_NEXT_Y       5 + B_REC_Y + B_REC_HEIGHT
#define B_NEXT_WIDTH   B_OPEN_WIDTH/2 - 1 
#define B_NEXT_HEIGHT  B_OPEN_HEIGHT
#define B_NEXT_STYLE   WS_VISIBLE | WS_CHILD //| WS_DISABLED
#define B_NEXT_TEXT    ">"
#define B_NEXT_ID      310
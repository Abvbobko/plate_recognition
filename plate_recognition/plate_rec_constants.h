#pragma once
// CONSTANTS
// window size
#define MIN_WINDOW_WIDTH   850
#define MAX_WINDOW_WIDTH   850
#define MIN_WINDOW_HEIGHT  410
#define MAX_WINDOW_HEIGHT  410

// BUTTONS
#define STANDART_BUTTON_WIDTH     220
#define STANDART_BUTTON_HEIGHT    74
#define DISTANCE_BETWEEN_BUTTONS  5
// button OPEN
#define B_OPEN_X       605
#define B_OPEN_Y       9
#define B_OPEN_WIDTH   (STANDART_BUTTON_WIDTH/2 - 1) 
#define B_OPEN_HEIGHT  STANDART_BUTTON_HEIGHT
#define B_OPEN_STYLE   (WS_VISIBLE | WS_CHILD)
#define B_OPEN_TEXT    "Open"
#define B_OPEN_ID      100

//button SAVE
#define B_SAVE_X       605 + 2 + B_OPEN_WIDTH
#define B_SAVE_Y       9
#define B_SAVE_WIDTH   (STANDART_BUTTON_WIDTH/2 - 1) 
#define B_SAVE_HEIGHT  STANDART_BUTTON_HEIGHT
#define B_SAVE_STYLE   (WS_VISIBLE | WS_CHILD | WS_DISABLED)
#define B_SAVE_TEXT    "Save"
#define B_SAVE_ID      110

// button RECOGNIZE
#define B_REC_X       605
#define B_REC_Y       (DISTANCE_BETWEEN_BUTTONS + B_OPEN_Y + B_OPEN_HEIGHT)
#define B_REC_WIDTH   STANDART_BUTTON_WIDTH 
#define B_REC_HEIGHT  STANDART_BUTTON_HEIGHT
#define B_REC_STYLE   (WS_VISIBLE | WS_CHILD | WS_DISABLED)
#define B_REC_TEXT    "Recognize"
#define B_REC_ID      200

// button PREV
#define B_PREV_X       605
#define B_PREV_Y       (DISTANCE_BETWEEN_BUTTONS + B_REC_Y + B_REC_HEIGHT)
#define B_PREV_WIDTH   (STANDART_BUTTON_WIDTH/2 - 1)
#define B_PREV_HEIGHT  STANDART_BUTTON_HEIGHT
#define B_PREV_STYLE   (WS_VISIBLE | WS_CHILD | WS_DISABLED)
#define B_PREV_TEXT    "<"
#define B_PREV_ID      300

// button NEXT
#define B_NEXT_X       (605 + B_PREV_WIDTH + 2)
#define B_NEXT_Y       (DISTANCE_BETWEEN_BUTTONS + B_REC_Y + B_REC_HEIGHT)
#define B_NEXT_WIDTH   (STANDART_BUTTON_WIDTH/2 - 1)
#define B_NEXT_HEIGHT  STANDART_BUTTON_HEIGHT
#define B_NEXT_STYLE   (WS_VISIBLE | WS_CHILD | WS_DISABLED)
#define B_NEXT_TEXT    ">"
#define B_NEXT_ID      310

// RECTs & Text
// main rect and image size
#define MAIN_IMG_LEFT    10
#define MAIN_IMG_TOP     10
#define MAIN_IMG_RIGHT   600
#define MAIN_IMG_BOTTOM  400
// text inside main rect
#define MAIN_RECT_TEXT         L"No uploaded image."
#define MAIN_TEXT_START_POS_X  ((MAIN_IMG_RIGHT - MAIN_IMG_LEFT)/2.5)
#define MAIN_TEXT_START_POS_Y  ((MAIN_IMG_BOTTOM - MAIN_IMG_TOP)/2)

// plate rect and image size
#define PLATE_IMG_LEFT    B_OPEN_X
#define PLATE_IMG_TOP     5 + B_NEXT_Y + STANDART_BUTTON_HEIGHT
#define PLATE_IMG_RIGHT   (PLATE_IMG_LEFT + STANDART_BUTTON_WIDTH)
#define PLATE_IMG_BOTTOM  (PLATE_IMG_TOP + STANDART_BUTTON_HEIGHT)
// text inside plate rect
#define PLATE_RECT_TEXT         L"No recognized plate."
#define PLATE_TEXT_START_POS_X  (PLATE_IMG_LEFT + 35)
#define PLATE_TEXT_START_POS_Y  (PLATE_IMG_BOTTOM - PLATE_IMG_TOP - FONT_SIZE + 5)/2

// normalized plate rect and image size
#define NORM_IMG_LEFT    B_OPEN_X
#define NORM_IMG_TOP     DISTANCE_BETWEEN_BUTTONS + PLATE_IMG_BOTTOM
#define NORM_IMG_RIGHT   (NORM_IMG_LEFT + STANDART_BUTTON_WIDTH)
#define NORM_IMG_BOTTOM  (NORM_IMG_TOP + STANDART_BUTTON_HEIGHT)
// text inside plate rect
#define NORM_RECT_TEXT         L"No normalized plate."
#define NORM_TEXT_START_POS_X  (NORM_IMG_LEFT + 35)
#define NORM_TEXT_START_POS_Y  (NORM_IMG_BOTTOM - NORM_IMG_TOP - FONT_SIZE + 5)/2

// Errors
// classifiers errors
#define PLATE_CLASSIFIER_LOAD_ERROR       100
#define PLATE_CLASSIFIER_LOAD_ERROR_TEXT  L"Error. Can't load plate classifier."

#define SYMBOL_CLASSIFIER_LOAD_ERROR       110
#define SYMBOL_CLASSIFIER_LOAD_ERROR_TEXT  L"Error. Can't load symbol classifier."

// saving errors
#define SAVE_PLATE_ERROR        200
#define SAVE_PLATE_ERROR_TEXT   L"Error. Can't save license plate."
#define SAVE_NPLATE_ERROR       210
#define SAVE_NPLATE_ERROR_TEXT  L"Error. Can't save normalized license plate."

// open errors
#define FILE_OPEN_ERROR       300
#define FILE_OPEN_ERROR_TEXT  L"Error. Can't open file."
#define NO_LOADED_IMAGE       310
#define NO_LOADED_IMAGE_TEXT  L"Error. No loaded image."

// recognize errors
#define NO_RECOGNIZED_ELEMENTS_ERROR       400
#define NO_RECOGNIZED_ELEMENTS_ERROR_TEXT  L"Error. No recognized elements."
#define NO_PLATES_ERROR                    410
#define NO_PLATES_ERROR_TEXT               L"Error. No recognized plates."
#define NO_NPLATES_ERROR                   420
#define NO_NPLATES_ERROR_TEXT              L"Error. No normalized plates."

// ok or inknown errors
#define UNKNOWN_ERROR_TEXT L"Unknown error."
#define NO_ERRORS          0
#define NO_ERRORS_TEXT     L"OK"
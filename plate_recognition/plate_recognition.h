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

// constants
#define MIN_WINDOW_WIDTH  850
#define MAX_WINDOW_WIDTH  850

#define MIN_WINDOW_HEIGHT 450
#define MAX_WINDOW_HEIGHT 450
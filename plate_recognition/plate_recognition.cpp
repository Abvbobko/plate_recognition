﻿// plate_recognition.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "plate_recognition.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// window controller for operations with window
WindowController * winController;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PLATERECOGNITION, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PLATERECOGNITION));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PLATERECOGNITION));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PLATERECOGNITION);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   winController = new WindowController();
//   recTools = new RecognitionTools();

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//

// buttons
HWND openButton, saveButton, recButton, prevButton, nextButton;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		
		// Create all buttons on window
		openButton = CreateWindow(TEXT("button"), TEXT(B_OPEN_TEXT),
			B_OPEN_STYLE, B_OPEN_X, B_OPEN_Y, 
			B_OPEN_WIDTH, B_OPEN_HEIGHT,
			hWnd, (HMENU)B_OPEN_ID, NULL, NULL);

		saveButton = CreateWindow(TEXT("button"), TEXT(B_SAVE_TEXT),
			B_SAVE_STYLE, B_SAVE_X, B_SAVE_Y,
			B_SAVE_WIDTH, B_SAVE_HEIGHT,
			hWnd, (HMENU)B_SAVE_ID, NULL, NULL);

		recButton = CreateWindow(TEXT("button"), TEXT(B_REC_TEXT),
			B_REC_STYLE, B_REC_X, B_REC_Y,
			B_REC_WIDTH, B_REC_HEIGHT,
			hWnd, (HMENU)B_REC_ID, NULL, NULL);

		prevButton = CreateWindow(TEXT("button"), TEXT(B_PREV_TEXT),
			B_PREV_STYLE, B_PREV_X, B_PREV_Y,
			B_PREV_WIDTH, B_PREV_HEIGHT,
			hWnd, (HMENU)B_PREV_ID, NULL, NULL);

		nextButton = CreateWindow(TEXT("button"), TEXT(B_NEXT_TEXT),
			B_NEXT_STYLE, B_NEXT_X, B_NEXT_Y,
			B_NEXT_WIDTH, B_NEXT_HEIGHT,
			hWnd, (HMENU)B_NEXT_ID, NULL, NULL);
		break;

    case WM_COMMAND:
		{
            int wmId = LOWORD(wParam);            
            switch (wmId)
            {
			case B_OPEN_ID: 
				{
					// show open file dialog
					OPENFILENAME arg = {};
					arg.lStructSize = sizeof(arg);
					TCHAR file[1024];
					file[0] = '\0';
					arg.lpstrFile = file;
					arg.nMaxFile = 1024;
					arg.Flags = OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
					arg.lpstrFilter = L"Supported Files(*.jpg, *.png, *.bmp)\0*.jpg;*.png;*.bmp\0";
					if (GetOpenFileName(&arg)) {
						char filePath[1024];
						size_t charsConverted = 0;
						wcstombs_s(NULL, filePath, arg.lpstrFile, 1024);
						// open file and set image
						winController->SetImage(filePath);								
					}							
					// for showing image we must redraw window
					RedrawWindow(hWnd, 0, 0, RDW_INVALIDATE);
					if (winController->GetError()) {
						// check results of file uploading
						MessageBox(hWnd, winController->GetErrorText(winController->GetError()), L"Ok", MB_OK);
					}
					else {
						// if no errors, activate buttons
						EnableWindow(recButton, true);
						EnableWindow(saveButton, false);
					}
				}
				break;
			case B_REC_ID:
				{				
					// license plate recognizing
					bool result = winController->Recognize();
					if (result) {
						// result of recognizing
						EnableWindow(saveButton, true);
						// if we have many numbers (and pages)
						if (winController->GetPagesNum() < 2) {
							EnableWindow(prevButton, false);
							EnableWindow(nextButton, false);
						}
						else {
							EnableWindow(nextButton, true);
						}
					}
					else {
						EnableWindow(saveButton, false);
					}
					if (winController->GetError()) {
						MessageBox(hWnd, winController->GetErrorText(winController->GetError()), L"Ok", MB_OK);
					}
					RedrawWindow(hWnd, 0, 0, RDW_INVALIDATE);
				}
				break;
			case B_SAVE_ID: 
				{
					// open save file dialog
					OPENFILENAME arg = {};
					arg.lStructSize = sizeof(arg);
					TCHAR file[1024];
					file[0] = '\0';
					arg.lpstrFile = file;
					arg.nMaxFile = 1024;
					arg.Flags = OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_PATHMUSTEXIST;
					arg.lpstrDefExt = L"jpg";
					arg.lpstrFilter = L"Supported Files(*.jpg, *.png, *.bmp)\0*.jpg;*.png;*.bmp\0";
					if (GetSaveFileName(&arg)) {
						// save main license plate
						char filePath[1024];
						size_t charsConverted = 0;
						wcstombs_s(NULL, filePath, arg.lpstrFile, 1024);
						winController->SavePlate(filePath);					
					}
					if (winController->GetError()) {
						MessageBox(hWnd, winController->GetErrorText(winController->GetError()), L"Ok", MB_OK);
					}
					if (GetSaveFileName(&arg)) {
						// save normalized license plate
						char filePath[1024];
						size_t charsConverted = 0;
						wcstombs_s(NULL, filePath, arg.lpstrFile, 1024);
						winController->SaveNPlate(filePath);
					}
					if (winController->GetError()) {
						MessageBox(hWnd, winController->GetErrorText(winController->GetError()), L"Ok", MB_OK);
					}
				
				}
				break;
			case B_NEXT_ID:
				// open next license plate from photo (next page)
				winController->IncPage();
				if (winController->IsPagesEnd()) {
					EnableWindow(nextButton, false);
					EnableWindow(prevButton, true);
				}
				RedrawWindow(hWnd, 0, 0, RDW_INVALIDATE);
				break;
			case B_PREV_ID:
				// open prev license plate from photo (prev page)
				winController->DecPage();				
				if (winController->IsPagesStart()) {
					EnableWindow(prevButton, false);
					EnableWindow(nextButton, true);
				}
				
				RedrawWindow(hWnd, 0, 0, RDW_INVALIDATE);
				break;			
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;			
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	
    case WM_PAINT:
        {
			// write rectangles on window (or photos) 
			PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);			
			winController->DrawComponents(hdc);			
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

	case WM_GETMINMAXINFO: {										
		// set size of window
		RECT w, c;
		
		GetWindowRect(hWnd, &w);
		GetClientRect(hWnd, &c);

		LPMINMAXINFO pMinMaxInfo = (LPMINMAXINFO)lParam;
		pMinMaxInfo->ptMinTrackSize.x = MIN_WINDOW_WIDTH;
		pMinMaxInfo->ptMaxTrackSize.x = MAX_WINDOW_WIDTH;

		int header = (w.bottom - w.top) - (c.bottom - c.top);
		pMinMaxInfo->ptMinTrackSize.y = MIN_WINDOW_HEIGHT + header;
		pMinMaxInfo->ptMaxTrackSize.y = MAX_WINDOW_HEIGHT + header;
				
		}
		break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

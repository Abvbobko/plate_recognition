// plate_recognition.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "plate_recognition.h"


#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

WindowController * winController;
RecognitionTools * recTools;

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
   recTools = new RecognitionTools();

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		
		CreateWindow(TEXT("button"), TEXT(B_OPEN_TEXT),
			B_OPEN_STYLE,
			B_OPEN_X, B_OPEN_Y, 
			B_OPEN_WIDTH, B_OPEN_HEIGHT,
			hWnd, (HMENU)B_OPEN_ID, NULL, NULL);

		CreateWindow(TEXT("button"), TEXT(B_REC_TEXT),
			B_REC_STYLE,
			B_REC_X, B_REC_Y,
			B_REC_WIDTH, B_REC_HEIGHT,
			hWnd, (HMENU)B_REC_ID, NULL, NULL);

		CreateWindow(TEXT("button"), TEXT(B_PREV_TEXT),
			B_PREV_STYLE,
			B_PREV_X, B_PREV_Y,
			B_PREV_WIDTH, B_PREV_HEIGHT,
			hWnd, (HMENU)B_PREV_ID, NULL, NULL);

		CreateWindow(TEXT("button"), TEXT(B_NEXT_TEXT),
			B_NEXT_STYLE,
			B_NEXT_X, B_NEXT_Y,
			B_NEXT_WIDTH, B_NEXT_HEIGHT,
			hWnd, (HMENU)B_NEXT_ID, NULL, NULL);

		break;
    case WM_COMMAND:
        {		
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
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
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			winController->DrawImageRect(hdc, recTools->GetImage(), 
				MAIN_IMG_LEFT, MAIN_IMG_TOP, MAIN_IMG_RIGHT, MAIN_IMG_BOTTOM, 
				MAIN_RECT_TEXT, MAIN_TEXT_START_POS_X, MAIN_TEXT_START_POS_Y);

			winController->DrawImageRect(hdc, recTools->GetImage(), //// тут надо сделать не гет image, а гет плейтс
				// нужно вставить if есть фотки нету фоток, а лучше перенести в winController
				PLATE_IMG_LEFT, PLATE_IMG_TOP, PLATE_IMG_RIGHT, PLATE_IMG_BOTTOM,
				PLATE_RECT_TEXT, PLATE_TEXT_START_POS_X, PLATE_TEXT_START_POS_Y);
			// TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:

        PostQuitMessage(0);
        break;

	case WM_GETMINMAXINFO: {										
		
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

// LTW_midterm.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "LTW_midterm.h"
#include <windowsx.h>
#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>
#include <string>
#include <commctrl.h>
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")

#define CX_ICON  32 
#define CY_ICON  32 
#define NUM_ICONS  6

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HIMAGELIST himlIcons;
// Forward declarations of functions included in this code module:
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

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LTWMIDTERM, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LTWMIDTERM));

    MSG msg;

    // Main message loop:
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LTWMIDTERM));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LTWMIDTERM);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, L"Ngoãn Ný Chi Tiêu", WS_OVERLAPPEDWINDOW,
	   10, 10, 10000, 10000, nullptr, nullptr, hInstance, nullptr); 

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//Create Image list
HIMAGELIST AddIconsToImageList(HINSTANCE hinst)
{
	HICON hicon;           // handle to icon 

						   // Ensure that the common control DLL is loaded. 
	InitCommonControls();

	// Create a masked image list large enough to hold the icons. 
	himlIcons = ImageList_Create(CX_ICON, CY_ICON, ILC_MASK, NUM_ICONS, 0);

	// Load the icon resources, and add the icons to the image list. 
	hicon = LoadIcon(hinst, MAKEINTRESOURCE(IDI_FOOD));
	ImageList_AddIcon(himlIcons, hicon);

	hicon = LoadIcon(hinst, MAKEINTRESOURCE(IDI_TRANS));
	ImageList_AddIcon(himlIcons, hicon);

	hicon = LoadIcon(hinst, MAKEINTRESOURCE(IDI_HOUSE));
	ImageList_AddIcon(himlIcons, hicon);

	hicon = LoadIcon(hinst, MAKEINTRESOURCE(IDI_BATTERY));
	ImageList_AddIcon(himlIcons, hicon);

	hicon = LoadIcon(hinst, MAKEINTRESOURCE(IDI_SOAP));
	ImageList_AddIcon(himlIcons, hicon);

	hicon = LoadIcon(hinst, MAKEINTRESOURCE(IDI_SERVICE));
	ImageList_AddIcon(himlIcons, hicon);

	return himlIcons;
}
//
//Type of Expense
TCHAR *item[] = { L"Ăn uống" ,L"Di chuyển (bus, xăng, taxi…)" , L"Nhà cửa (tiền thuê nhà, nước, điện,…)" , L"Xe cộ (xăng, vá xe, bảo dưỡng…)",
L"Nhu yếu phẩm (xà bông, sữa tắm…)", L"Dịch vụ (intenet, thẻ cảo điện thoại…)" };

int sum = 0;
float Food = 0, Transport = 0, House = 0, Car = 0, Soap = 0, Service = 0;
int iCount = 0;
int iIndex[100];
int iCountFile;
HWND cbbTypeofExpense;
HWND hInfo, hMoney, hSum;
HWND btnADD;
HWND hFood, hTransport, hHouse, hCar, hSoap, hService;
HWND hwnd;
HWND hListView;

//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
	{
		INITCOMMONCONTROLSEX icc;
		icc.dwSize = sizeof(icc);
		icc.dwICC = ICC_USEREX_CLASSES;
		InitCommonControlsEx(&icc);
		 
		// Lấy font hệ thống
		LOGFONT lf;
		GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
		HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
			lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
			lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
			lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
			lf.lfPitchAndFamily, lf.lfFaceName);

		HFONT hFont1 = CreateFont(24, 16,
			lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
			lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
			lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
			lf.lfPitchAndFamily, lf.lfFaceName);

		AddIconsToImageList(hInst);
		HWND titleOfChart = CreateWindowEx(0, WC_STATIC, L"BIỂU ĐỒ QUẢN LÝ CHI TIÊU",
			SS_CENTER | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
			850, 35, 550, 50, hWnd, NULL, hInst, NULL);
		SendMessage(titleOfChart, WM_SETFONT, WPARAM(hFont1), TRUE);

		HWND CT = CreateWindowEx(0, WC_BUTTON, L"Thêm chi tiêu", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
			10, 40, 800, 150, hWnd, (HMENU)0, hInst, NULL);
		SendMessage(CT, WM_SETFONT, WPARAM(hFont + 2), TRUE);
		break;
	}
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
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
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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


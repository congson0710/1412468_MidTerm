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
//Type of Expense
TCHAR *item[] = { L"Ăn uống" ,L"Di chuyển (bus, xăng, taxi…)" , L"Nhà cửa (tiền thuê nhà, nước, điện,…)" , L"Xe cộ (xăng, vá xe, bảo dưỡng…)",
L"Nhu yếu phẩm (xà bông, sữa tắm…)", L"Dịch vụ (intenet, thẻ cảo điện thoại…)" };

int sum = 0;
float Food = 0, Transport = 0, House = 0, Car = 0, Soap = 0, Service = 0;
int iCount = 0;
int iIndex[100];
int iCountFile;
HWND comboBox;
HWND hInfo, hMoney, hSum;
HWND btnADD;
HWND hFood, hTransport, hHouse, hCar, hSoap, hService;
HWND hwnd;
HWND listView;
//
//add item to combo box
bool addItemComboBox(HWND comboBox) {
	COMBOBOXEXITEM cbitem;

	cbitem.mask = CBEIF_TEXT | CBEIF_INDENT;

	for (int iCount = 0; iCount < 6; iCount++) {
		cbitem.iItem = iCount;
		cbitem.pszText = item[iCount];
		cbitem.cchTextMax = 128;
		cbitem.iIndent = 0;

		if (SendMessage(comboBox, CBEM_INSERTITEM, 0, (LPARAM)&cbitem) == -1)
			return false;
	}
	return true;
}
//
void InitCol()
{
	LVCOLUMN lvColName;
	lvColName.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;

	lvColName.fmt = LVCFMT_LEFT;
	lvColName.cx = 300;
	lvColName.pszText = _T("Loại chi tiêu");
	ListView_InsertColumn(listView, 0, &lvColName);

	LVCOLUMN lvColType;
	lvColType.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColType.cx = 350;
	lvColType.fmt = LVCFMT_LEFT;
	lvColType.pszText = _T("Nội dung");
	ListView_InsertColumn(listView, 1, &lvColType);

	LVCOLUMN lvColSize;
	lvColSize.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColSize.cx = 250;
	lvColSize.fmt = LVCFMT_LEFT;
	lvColSize.pszText = _T("Số tiền");
	ListView_InsertColumn(listView, 2, &lvColSize);
}
//
//
void DrawChart(HWND hWnd, HDC hdc, HBRUSH hBrush, double AngleStart, double AngleEnd) {
	hdc = GetDC(hWnd);
	BeginPath(hdc);
	SelectObject(hdc, hBrush);
	MoveToEx(hdc, 1100, 300, (LPPOINT)NULL);
	AngleArc(hdc, 1100, 300, 175, AngleStart, -AngleEnd);
	LineTo(hdc, 1100, 300);
	EndPath(hdc);
	StrokeAndFillPath(hdc);
	ReleaseDC(hWnd, hdc);
}
//
void WriteFile()
{
	wofstream fout;
	fout.imbue(loc);
	fout.open(L"data.txt");
	fout << iCount << endl;
	WCHAR buffer1[255];
	WCHAR buffer2[255];
	WCHAR buffer3[255];
	for (int i = 0; i < iCount; i++)
	{
		fout << iIndex[i] << endl;
		ListView_GetItemText(listView, i, 0, buffer1, 255);
		ListView_GetItemText(listView, i, 1, buffer2, 255);
		ListView_GetItemText(listView, i, 2, buffer3, 255);
		fout << buffer1 << endl;
		fout << buffer2 << endl;
		fout << buffer3 << endl;
	}

	fout.close();
}

using namespace std;
locale loc(locale(), new codecvt_utf8<wchar_t>);

void ReadFile()
{
	wifstream fin(L"data.txt");
	fin.imbue(loc);
	WCHAR buffer1[2];
	WCHAR buffer5[2];
	WCHAR buffer2[255];
	WCHAR buffer3[255];
	WCHAR buffer4[255];
	fin.getline(buffer1, 2, '\n');
	iCountFile = _wtoi(buffer1);
	for (int i = 0; i < iCountFile; i++)
	{
		fin.getline(buffer5, 2, '\n');
		fin.getline(buffer2, 255, '\n');
		fin.getline(buffer3, 255, '\n');
		fin.getline(buffer4, 255, '\n');

		LV_ITEM lv;
		lv.mask = LVIF_TEXT;
		lv.iItem = i;

		lv.iSubItem = 0;
		lv.pszText = buffer2;
		ListView_InsertItem(listView, &lv);

		ListView_SetItemText(listView, i, 1, buffer3);
		ListView_SetItemText(listView, i, 2, buffer4);
		iIndex[i] = _wtoi(buffer5);
		int plus = _wtoi(buffer4);
		SumEachType(_wtoi(buffer5), plus);
		sum += plus;
	}
	iCount = iCountFile;
	WCHAR *bfSum = new TCHAR[255];
	wsprintf(bfSum, L"Tổng chi tiêu: %d", sum);
	SetWindowText(hSum, bfSum);
	WCHAR *bfFood = new TCHAR[255];
	WCHAR *bfTrans = new TCHAR[255];
	WCHAR *bfHouse = new TCHAR[255];
	WCHAR *bfCar = new TCHAR[255];
	WCHAR *bfSoap = new TCHAR[255];
	WCHAR *bfServ = new TCHAR[255];
	float dFood = ((Food * 100) / sum);
	swprintf(bfFood, 255, L"%1.f", dFood);
	SetWindowText(hFood, bfFood);
	float dTrans = ((Transport * 100) / sum);
	swprintf(bfTrans, 255, L"%1.f", dTrans);
	SetWindowText(hTransport, bfTrans);
	float dHouse = ((House * 100) / sum);
	swprintf(bfHouse, 255, L"%1.f", dHouse);
	SetWindowText(hHouse, bfHouse);
	float dCar = ((Car * 100) / sum);
	swprintf(bfCar, 255, L"%1.f", dCar);
	SetWindowText(hCar, bfCar);
	float dSoap = ((Soap * 100) / sum);
	swprintf(bfSoap, 255, L"%1.f", dSoap);
	SetWindowText(hSoap, bfSoap);
	float dServ = ((Service * 100) / sum);
	swprintf(bfServ, 255, L"%1.f", dServ);
	SetWindowText(hService, bfServ);
}
//
void SumEachType(int sel, int value)
{
	switch (sel)
	{
	case 0:
		Food += value;
		break;
	case 1:
		Transport += value;
		break;
	case 2:
		House += value;
		break;
	case 3:
		Car += value;
		break;
	case 4:
		Soap += value;
		break;
	case 5:
		Service += value;
		break;
	}
}
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
		// chart name
		HWND titleOfChart = CreateWindowEx(0, WC_STATIC, L"BIỂU ĐỒ QUẢN LÝ CHI TIÊU",
			SS_CENTER | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
			850, 35, 550, 50, hWnd, NULL, hInst, NULL);
		SendMessage(titleOfChart, WM_SETFONT, WPARAM(hFont1), TRUE);
		// button consume
		HWND CT = CreateWindowEx(0, WC_BUTTON, L"Thêm chi tiêu", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
			10, 40, 800, 150, hWnd, (HMENU)0, hInst, NULL);
		SendMessage(CT, WM_SETFONT, WPARAM(hFont + 2), TRUE);
		// combobox for choice
		comboBox = CreateWindowEx(0, WC_COMBOBOXEX, NULL,
			CBS_DROPDOWN | CBS_DROPDOWNLIST | CBS_SIMPLE | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
			30, 100, 200, 200, hWnd, NULL, hInst, NULL);
		SendMessage(comboBox, WM_SETFONT, WPARAM(hFont + 1), TRUE);
		addItemComboBox(comboBox);

		//Create infomation edit box
		HWND type = CreateWindowEx(0, WC_STATIC, L"Loại chi tiêu:",
			SS_LEFT | WS_CHILD | WS_VISIBLE,
			30, 70, 150, 30, hWnd, NULL, hInst, NULL);
		SendMessage(type, WM_SETFONT, WPARAM(hFont + 1), TRUE);

		HWND info = CreateWindowEx(0, WC_STATIC, L"Nội dung:",
			SS_LEFT | WS_CHILD | WS_VISIBLE,
			320, 70, 150, 30, hWnd, NULL, hInst, NULL);
		SendMessage(info, WM_SETFONT, WPARAM(hFont + 1), TRUE);

		HWND money = CreateWindowEx(0, WC_STATIC, L"Số tiền:",
			SS_LEFT | WS_CHILD | WS_VISIBLE,
			540, 70, 150, 30, hWnd, NULL, hInst, NULL);
		SendMessage(money, WM_SETFONT, WPARAM(hFont + 1), TRUE);

		//Create information fill box
		hInfo = CreateWindowEx(0, WC_EDIT, NULL,
			ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_VSCROLL | WS_BORDER,
			320, 100, 180, 30, hWnd, NULL, hInst, NULL);
		SendMessage(hInfo, WM_SETFONT, WPARAM(hFont + 1), TRUE);

		hMoney = CreateWindowEx(0, WC_EDIT, NULL,
			ES_LEFT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | ES_NUMBER | WS_BORDER,
			540, 100, 150, 30, hWnd, NULL, hInst, NULL);
		SendMessage(hMoney, WM_SETFONT, WPARAM(hFont + 1), TRUE);

		btnADD = CreateWindowEx(0, WC_BUTTON, L"Thêm",
			BS_PUSHBUTTON | BS_TEXT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
			650, 140, 100, 35, hWnd, (HMENU)IDC_BTNADD, hInst, NULL);
		SendMessage(hwnd, WM_SETFONT, WPARAM(hFont + 2), TRUE);

		hSum = CreateWindowEx(0, WC_EDIT, NULL,
			ES_LEFT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | ES_READONLY,
			550, 665, 300, 50, hWnd, NULL, hInst, NULL);
		SendMessage(hSum, WM_SETFONT, WPARAM(hFont + 1), TRUE);

		//Create menu pie chart
		hFood = CreateWindowEx(0, WC_STATIC, L"",
			SS_CENTER | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
			850, 500, 50, 50, hWnd, NULL, hInst, NULL);

		hwnd = CreateWindowEx(0, WC_STATIC, L"Ăn uống",
			SS_LEFT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
			900, 500, 100, 50, hWnd, NULL, hInst, NULL);
		SendMessage(hwnd, WM_SETFONT, WPARAM(hFont + 1), TRUE);

		hTransport = CreateWindowEx(0, WC_STATIC, L"",
			SS_CENTER | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
			1050, 500, 50, 50, hWnd, NULL, hInst, NULL);

		hwnd = CreateWindowEx(0, WC_STATIC, L"Di chuyển",
			SS_LEFT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
			1100, 500, 100, 50, hWnd, NULL, hInst, NULL);
		SendMessage(hwnd, WM_SETFONT, WPARAM(hFont + 1), TRUE);

		hHouse = CreateWindowEx(0, WC_STATIC, L"",
			SS_CENTER | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
			1250, 500, 50, 50, hWnd, NULL, hInst, NULL);

		hwnd = CreateWindowEx(0, WC_STATIC, L"Nhà cửa",
			SS_LEFT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
			1300, 500, 100, 50, hWnd, NULL, hInst, NULL);
		SendMessage(hwnd, WM_SETFONT, WPARAM(hFont + 1), TRUE);

		hCar = CreateWindowEx(0, WC_STATIC, L"",
			SS_CENTER | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
			850, 600, 50, 50, hWnd, NULL, hInst, NULL);

		hwnd = CreateWindowEx(0, WC_STATIC, L"Xe cộ",
			SS_LEFT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
			900, 600, 100, 50, hWnd, NULL, hInst, NULL);
		SendMessage(hwnd, WM_SETFONT, WPARAM(hFont + 1), TRUE);

		hSoap = CreateWindowEx(0, WC_STATIC, L"",
			SS_CENTER | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
			1050, 600, 50, 50, hWnd, NULL, hInst, NULL);
		hwnd = CreateWindowEx(0, WC_STATIC, L"Nhu yếu phẩm",
			SS_LEFT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
			1100, 600, 100, 50, hWnd, NULL, hInst, NULL);
		SendMessage(hwnd, WM_SETFONT, WPARAM(hFont + 1), TRUE);

		hService = CreateWindowEx(0, WC_STATIC, L"",
			SS_CENTER | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
			1250, 600, 50, 50, hWnd, NULL, hInst, NULL);

		hwnd = CreateWindowEx(0, WC_STATIC, L"Dịch vụ",
			SS_LEFT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
			1300, 600, 100, 50, hWnd, NULL, hInst, NULL);
		SendMessage(hwnd, WM_SETFONT, WPARAM(hFont + 1), TRUE);
		//Create listview
		InitCommonControls();
		HWND hlv = CreateWindowEx(0, WC_BUTTON, L"Danh sách các chi tiêu",
			WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
			10, 200, 800, 450, hWnd, (HMENU)0, hInst, NULL);
		SendMessage(hwnd, WM_SETFONT, WPARAM(hFont + 2), TRUE);

		listView = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, L"List view",
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | LVS_REPORT |
			LVS_EDITLABELS | LVS_SHOWSELALWAYS,
			30, 230, 750, 400, hWnd, NULL, hInst, NULL);
		InitCol();
		ReadFile();
		//Paint
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (lParam == (LPARAM)hFood)
		{
			HDC hdcStatic = (HDC)wParam;
			SetTextColor(hdcStatic, RGB(255, 255, 255));
			SetBkColor(hdcStatic, RGB(249, 244, 0));
			return (INT_PTR)CreateSolidBrush(RGB(249, 244, 0));
		}
		else if (lParam == (LPARAM)hTransport)
		{
			HDC hdcStatic = (HDC)wParam;
			SetTextColor(hdcStatic, RGB(255, 255, 255));
			SetBkColor(hdcStatic, RGB(223, 0, 41));
			return (INT_PTR)CreateSolidBrush(RGB(223, 0, 41));
		}
		else if (lParam == (LPARAM)hHouse)
		{
			HDC hdcStatic = (HDC)wParam;
			SetTextColor(hdcStatic, RGB(255, 255, 255));
			SetBkColor(hdcStatic, RGB(32, 90, 167));
			return (INT_PTR)CreateSolidBrush(RGB(32, 90, 167));
		}
		else if (lParam == (LPARAM)hCar)
		{
			HDC hdcStatic = (HDC)wParam;
			SetTextColor(hdcStatic, RGB(255, 255, 255));
			SetBkColor(hdcStatic, RGB(91, 189, 43));
			return (INT_PTR)CreateSolidBrush(RGB(91, 189, 43));
		}
		else if (lParam == (LPARAM)hSoap)
		{
			HDC hdcStatic = (HDC)wParam;
			SetTextColor(hdcStatic, RGB(255, 255, 255));
			SetBkColor(hdcStatic, RGB(255, 110, 180));
			return (INT_PTR)CreateSolidBrush(RGB(255, 110, 180));
		}
		else if (lParam == (LPARAM)hService)
		{
			HDC hdcStatic = (HDC)wParam;
			SetTextColor(hdcStatic, RGB(255, 255, 255));
			SetBkColor(hdcStatic, RGB(169, 169, 169));
			return (INT_PTR)CreateSolidBrush(RGB(169, 169, 169));
		}
		else
		{
			COLORREF RGBBuf = GetSysColor(COLOR_BTNFACE);
			HDC hdcStatic = (HDC)wParam;
			SetTextColor(hdcStatic, RGB(0, 0, 0));
			SetBkColor(hdcStatic, RGBBuf);
			return (INT_PTR)CreateSolidBrush(RGBBuf);
		}
		break;
	}
    case WM_COMMAND:
        {
			TCHAR* bfInfo = NULL;
			TCHAR* bfMoney = NULL;
			TCHAR* bfSum = NULL;

			int size2, size3;
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case IDC_BTNADD:
			{
				int sel = SendMessage(comboBox, CB_GETCURSEL, NULL, NULL);
				if (sel == -1) {
					MessageBox(hWnd, L"Hãy lựa chon một loại chi tiêu", L"Cảnh báo", 0);
					break;
				}
				iIndex[iCount] = sel;
				size2 = GetWindowTextLength(hInfo);
				size3 = GetWindowTextLength(hMoney);

				bfInfo = new TCHAR[size2 + 1];
				bfMoney = new TCHAR[size3 + 1];

				GetWindowText(hInfo, bfInfo, size2 + 1);
				GetWindowText(hMoney, bfMoney, size3 + 1);
				int plus = _wtoi(bfMoney);
				SumEachType(sel, plus);
				sum += plus;
				bfSum = new TCHAR[255];
				wsprintf(bfSum, L"Tổng chi tiêu: %d", sum);
				SetWindowText(hSum, bfSum);
				//Set percent to menu
				WCHAR *bfFood = new TCHAR[255];
				WCHAR *bfTrans = new TCHAR[255];
				WCHAR *bfHouse = new TCHAR[255];
				WCHAR *bfCar = new TCHAR[255];
				WCHAR *bfSoap = new TCHAR[255];
				WCHAR *bfServ = new TCHAR[255];
				float dFood = ((Food * 100) / sum);
				swprintf(bfFood, 255, L"%1.f", dFood);
				SetWindowText(hFood, bfFood);
				float dTrans = ((Transport * 100) / sum);
				swprintf(bfTrans, 255, L"%1.f", dTrans);
				SetWindowText(hTransport, bfTrans);
				float dHouse = ((House * 100) / sum);
				swprintf(bfHouse, 255, L"%1.f", dHouse);
				SetWindowText(hHouse, bfHouse);
				float dCar = ((Car * 100) / sum);
				swprintf(bfCar, 255, L"%1.f", dCar);
				SetWindowText(hCar, bfCar);
				float dSoap = ((Soap * 100) / sum);
				swprintf(bfSoap, 255, L"%1.f", dSoap);
				SetWindowText(hSoap, bfSoap);
				float dServ = ((Service * 100) / sum);
				swprintf(bfServ, 255, L"%1.f", dServ);
				SetWindowText(hService, bfServ);
				//Add item to list view
				LV_ITEM lv;
				lv.mask = LVIF_TEXT;
				lv.iItem = iCount;
				lv.iSubItem = 0;
				lv.pszText = item[sel];
				ListView_InsertItem(listView, &lv);
				ListView_SetItemText(listView, iCount, 1, bfInfo);
				ListView_SetItemText(listView, iCount, 2, bfMoney);
				SetWindowText(hInfo, L"");
				SetWindowText(hMoney, L"");
				iCount++;
				InvalidateRect(hWnd, NULL, TRUE);
				break;
			}
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
			HBRUSH hBrush1 = CreateSolidBrush(RGB(249, 244, 0));
			HBRUSH hBrush2 = CreateSolidBrush(RGB(223, 0, 41));
			HBRUSH hBrush3 = CreateSolidBrush(RGB(32, 90, 167));
			HBRUSH hBrush4 = CreateSolidBrush(RGB(91, 189, 43));
			HBRUSH hBrush5 = CreateSolidBrush(RGB(255, 110, 180));
			HBRUSH hBrush6 = CreateSolidBrush(RGB(169, 169, 169));
            HDC hdc = BeginPaint(hWnd, &ps);
			if (sum != 0)
			{
				double dFood = ((Food * 100) / sum)*3.6;
				DrawChart(hWnd, hdc, hBrush1, 90, dFood);

				double dTrans = ((Transport * 100) / sum)*3.6;
				DrawChart(hWnd, hdc, hBrush2, 90 - dFood, dTrans);

				double dHouse = ((House * 100) / sum)*3.6;
				DrawChart(hWnd, hdc, hBrush3, 90 - (dTrans + dFood), dHouse);

				double dCar = ((Car * 100) / sum)*3.6;
				DrawChart(hWnd, hdc, hBrush4, 90 - (dTrans + dFood + dHouse), dCar);

				double dSoap = ((Soap * 100) / sum)*3.6;
				DrawChart(hWnd, hdc, hBrush5, 90 - (dTrans + dFood + dHouse + dCar), dSoap);

				double dServ = ((Service * 100) / sum)*3.6;
				DrawChart(hWnd, hdc, hBrush6, 90 - (dTrans + dFood + dHouse + dCar + dSoap), dServ);
			}
            EndPaint(hWnd, &ps);
			DeleteBrush(hBrush1);
			DeleteBrush(hBrush2);
			DeleteBrush(hBrush3);
			DeleteBrush(hBrush4);
			DeleteBrush(hBrush5);
			DeleteBrush(hBrush6);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
		WriteFile();
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


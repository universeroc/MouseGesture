// Copyright (c) 2013 universeroc. All rights reserved.
//
// universeroc@gmail.com
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "mouse_gesture_view.h"

#include "resource.h"
#include "mouse_gesture_handler.h"
#include <ShellAPI.h>

NS_MG_START

#define WM_STATUS_TRAY_MSG WM_USER + 5

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void CreateStatusIcon(HWND hwnd);
void DestoryStatusIcon();

wchar_t MouseGestureView::title_[kMaxLoadString];
wchar_t MouseGestureView::class_name_[kMaxLoadString];

MouseGestureView::MouseGestureView(HINSTANCE instance,
    MouseGestureHandler* handler)
    : instance_(instance), handler_(handler) {
  LoadResource();
  RegisterWindowClass();
  Init();
}

MouseGestureView::~MouseGestureView(void) {
  DestoryStatusIcon();
}

void MouseGestureView::LoadResource() {
  // Initialize global strings
  LoadString(instance_, IDS_APP_TITLE, title_, kMaxLoadString);
  LoadString(instance_, IDC_MOUSE_GESTURE, class_name_, kMaxLoadString);
}

ATOM MouseGestureView::RegisterWindowClass() {
  WNDCLASSEX wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style			= CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc	= WndProc;
  wcex.cbClsExtra		= 0;
  wcex.cbWndExtra		= 0;
  wcex.hInstance		= instance_;
  wcex.hIcon			= LoadIcon(instance_, MAKEINTRESOURCE(IDI_MOUSE_GESTURE));
  wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
  wcex.lpszMenuName	= NULL;
  wcex.lpszClassName	= class_name_;
  wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  return RegisterClassEx(&wcex);
}

bool MouseGestureView::Init() {
  RECT rect;
  // we need a rect which size is same as desktop.
  GetWindowRect(GetDesktopWindow(), &rect);

  int x = rect.left;
  int y = rect.top;
  int w = rect.right - rect.left;
  int h = rect.bottom - rect.top;
  HWND hwnd = CreateWindowEx(
      WS_EX_LAYERED |
      WS_EX_TOOLWINDOW |
      WS_EX_TRANSPARENT | // Don't hittest this window
      WS_EX_TOPMOST,
      class_name_, title_,
      WS_POPUP,
      x, y, w, h, NULL, NULL, NULL, NULL);

  if (!hwnd)
    return false;

  SetLayeredWindowAttributes(hwnd,
      RGB(255, 255, 255),
      255,
      LWA_ALPHA | LWA_COLORKEY);
  
  hwnd_ = hwnd;

  SetWindowLong(hwnd, GWL_USERDATA, (long)instance_);

  CreateStatusIcon(hwnd);

  return true;
}

int MouseGestureView::RunLoop() {
  MSG msg;
  HACCEL hAccelTable;

  hAccelTable = LoadAccelerators(instance_, MAKEINTRESOURCE(IDC_MOUSE_GESTURE));

  // Main message loop:
  while (GetMessage(&msg, NULL, 0, 0))
  {
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
  return (int) msg.wParam;
}

void MouseGestureView::Update() {
  HDC hdc = GetDC(hwnd_);
  {
    //HBRUSH brush = CreateSolidBrush(RGB(0, 100, 100));
    HPEN pen = CreatePen(PS_SOLID, 5, RGB(0, 0, 255));
    //HBRUSH old_brush = (HBRUSH)SelectObject(hdc, brush);
    HPEN old_pen = (HPEN)SelectObject(hdc, pen);

    Polyline(hdc, &handler_->path()[0], handler_->path().size());

    // SelectObject(hdc, old_brush);
    SelectObject(hdc, old_pen);
    DeleteObject(pen);
  }   
  ReleaseDC(hwnd_, hdc);
}

LRESULT CALLBACK MouseGestureView::WndProc(
    HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT: {
      long instance = GetWindowLong(hWnd, GWL_USERDATA);
			DialogBox((HINSTANCE)instance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
      }
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
    // DestoryStatusIcon();
		PostQuitMessage(0);
		break;
  case WM_STATUS_TRAY_MSG:
    if(wParam == IDI_TRAY){
      if(lParam == WM_LBUTTONDOWN){
        OutputDebugString(L"status tray icon clicked!\n");
        long instance = GetWindowLong(hWnd, GWL_USERDATA);
        DialogBox((HINSTANCE)instance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About, );
        return TRUE;
      }	
    }
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

void CreateStatusIcon(HWND hwnd) {
  NOTIFYICONDATA data = {};
  data.cbSize = sizeof(NOTIFYICONDATA);
  data.hWnd = hwnd;
  data.uID = IDI_TRAY;
  data.uCallbackMessage = WM_STATUS_TRAY_MSG;
  data.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;  // never use dwInfoFlags!!!!
  wcscpy_s(data.szTip, MouseGestureView::class_name());

  long instance = GetWindowLong(hwnd, GWL_USERDATA);
  data.hIcon = LoadIcon((HINSTANCE)instance, MAKEINTRESOURCE(IDI_TRAY));

  Shell_NotifyIcon(NIM_ADD, &data);
}

void DestoryStatusIcon() {
  NOTIFYICONDATA data;
  Shell_NotifyIcon(NIM_DELETE, &data);
}

NS_MG_END
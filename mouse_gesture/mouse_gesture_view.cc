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

// if we use a local varible we could not remove the icon when app quit.
static NOTIFYICONDATA data = {};

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
  status_menu_ = LoadMenu(instance_, MAKEINTRESOURCE(MAKEWORD(IDC_MOUSE_GESTURE, 0)));

  // without this line below, you will not get a correct menu.
  // just a vertical line with blank content
  status_menu_ = GetSubMenu(status_menu_, 0);
}

ATOM MouseGestureView::RegisterWindowClass() {
  WNDCLASSEX wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style			= CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc	= WndProc;
  wcex.cbClsExtra		= 0;
  wcex.cbWndExtra		= 0;
  wcex.hInstance		= instance_;
  wcex.hIcon			= LoadIcon(instance_, MAKEINTRESOURCE(IDI_TRAY));
  wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
  wcex.lpszMenuName	= NULL;
  wcex.lpszClassName	= class_name_;
  wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_TRAY));

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

  SetWindowLong(hwnd, GWL_USERDATA, (long)(this));

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
    case IDM_SETTINGS: {
      MouseGestureView* view =
          (MouseGestureView*)GetWindowLong(hWnd, GWL_USERDATA);
      DialogBox(view->instance(), MAKEINTRESOURCE(IDD_DIALOG_SETTINGS), hWnd, About);
      break;
      }
		case IDM_ABOUT: {
      MouseGestureView* view =
          (MouseGestureView*)GetWindowLong(hWnd, GWL_USERDATA);
			DialogBox(view->instance(), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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
    DestoryStatusIcon();
    PostQuitMessage(0);
		break;
  case WM_QUIT:
    break;
  case WM_CLOSE:
    break;
  case WM_STATUS_TRAY_MSG:
    if(wParam == IDI_TRAY){
      if(lParam == WM_LBUTTONDOWN || lParam == WM_RBUTTONDOWN){
        MouseGestureView* view =
            (MouseGestureView*)GetWindowLong(hWnd, GWL_USERDATA);
        POINT pt;
        GetCursorPos(&pt);
        ::SetForegroundWindow(hWnd);  // for lost focus to dispop menu.
        TrackPopupMenu(view->status_menu_,
            TPM_LEFTALIGN | TPM_TOPALIGN/* | TPM_RETURNCMD*/,
            pt.x, pt.y, 0, view->hwnd_, 0);
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
  data.cbSize = sizeof(NOTIFYICONDATA);
  data.hWnd = hwnd;
  data.uID = IDI_TRAY;
  data.uCallbackMessage = WM_STATUS_TRAY_MSG;
  data.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;  // never use dwInfoFlags!!!!
  wcscpy_s(data.szTip, MouseGestureView::class_name());

  MouseGestureView* view =
      (MouseGestureView*)GetWindowLong(hwnd, GWL_USERDATA);
  data.hIcon = LoadIcon(view->instance(), MAKEINTRESOURCE(IDI_TRAY));

  Shell_NotifyIcon(NIM_ADD, &data);
}

void DestoryStatusIcon() {
  Shell_NotifyIcon(NIM_DELETE, &data);
}

NS_MG_END
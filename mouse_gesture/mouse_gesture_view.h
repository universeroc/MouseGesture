// Copyright (c) 2013 universeroc. All rights reserved.
//
// universeroc@gmail.com
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __MOUSE_GESTURE_VIEW_H__
#define __MOUSE_GESTURE_VIEW_H__

#include <windows.h>
#include "../base/namespace.h"

NS_MG_START

class MouseGestureHandler;

class MouseGestureView {
public:
  MouseGestureView(HINSTANCE instance, MouseGestureHandler* handler);
  ~MouseGestureView(void);

  HWND hwnd() { return hwnd_; }
  int RunLoop();
  void Update();

  static wchar_t* title() { return title_; }
  static wchar_t* class_name() { return class_name_; }

private:
  void LoadResource();
  ATOM RegisterWindowClass();
  bool Init();

  static LRESULT CALLBACK WndProc(
      HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

  static const int kMaxLoadString = 100;

  static wchar_t title_[kMaxLoadString];
  static wchar_t class_name_[kMaxLoadString];

  HWND hwnd_;
  HINSTANCE instance_;

  MouseGestureHandler* handler_;
};

NS_MG_END

#endif

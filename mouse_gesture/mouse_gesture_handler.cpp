// Copyright (c) 2013 universeroc. All rights reserved.
//
// universeroc@gmail.com
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "mouse_gesture_handler.h"

#include <string.h>
#include "mouse_gesture_view.h"
#include "../mouse_gesture_command_executor/mouse_gesture_command_executor_impl.h"

#ifndef LOG
#define LOG(A) OutputDebugString(A)
#endif

NS_MG_START

MouseGestureHandler::MouseGestureHandler(HINSTANCE instance)
    : right_button_down_(false),
      view_(new MouseGestureView(instance, this)),
      event_hooker_(new MouseEventHooker(this)),
      command_executor_(new MouseGestureCommandExecutorImpl(this)) {
}

MouseGestureHandler::~MouseGestureHandler(void){
  delete view_;
  delete event_hooker_;
  delete command_executor_;
}

int MouseGestureHandler::Run() {
  return view_->RunLoop();
}

bool MouseGestureHandler::OnMouseEvent(UINT event, const MSLLHOOKSTRUCT& ms) {
  switch (event) {
    case WM_RBUTTONDOWN: {
      OnRButtonDown(ms.pt);
      return true;
    }
    case WM_RBUTTONUP: {
      OnRButtonUp(ms.pt);
      InvalidateRect(view_->hwnd(), NULL, TRUE);
      ShowWindow(view_->hwnd(), SW_HIDE);
      return true;
    }
    case WM_MOUSEMOVE: {
      if (right_button_down_) {
        OnMouseMove(ms.pt);
        // If we return true, the mouse would be frozen, :P
        return false;
      }
    }
  }
  return false;
}

HWND MouseGestureHandler::GetNativeWindow() {
  POINT pt;
  GetCursorPos(&pt);
  HWND hwnd = WindowFromPoint(pt);
  current_hwnd_ = hwnd;

  // We want the top-most hwnd
  while (GetParent(hwnd)) {
    hwnd = GetParent(hwnd);
  }
  return hwnd;
}

void MouseGestureHandler::OnRButtonDown(const POINT& pt) {
  right_button_down_ = true;
  path_.push_back(pt);
  recognizor_.FromPoint(pt);
  current_topmost_hwnd_ = GetNativeWindow();

  SetWindowPos(view_->hwnd(),
               HWND_TOPMOST,
               0, 0, 0, 0,
               SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW | SWP_NOACTIVATE);
}

void MouseGestureHandler::OnMouseMove(const POINT& pt) {
  path_.push_back(pt);
  recognizor_.OnRecognize(pt);
  view_->Update();
}

void MouseGestureHandler::OnRButtonUp(const POINT& pt) {
  right_button_down_ = false;
  command_executor_->DispatchDirectionToExecutor(recognizor_.direction());
  path_.clear();
}

NS_MG_END

// Copyright (c) 2013 universeroc. All rights reserved.
//
// universeroc@gmail.com
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __MOUSE_EVENT_HOOCKER_H__
#define __MOUSE_EVENT_HOOCKER_H__

#include "../base/data.h"
#include "../base/namespace.h"
#include <windows.h>

#ifndef _USRDLL
#define MOUSE_GESTURE_EXPORT __declspec(dllimport)
#else
#define MOUSE_GESTURE_EXPORT __declspec(dllexport)
#endif

NS_MG_START

class MOUSE_GESTURE_EXPORT MouseEventHookerDelegate {
public:
  virtual ~MouseEventHookerDelegate() {}

  virtual bool OnMouseEvent(UINT event, const MSLLHOOKSTRUCT& ms) = 0;

  virtual HWND current_hwnd() const = 0;

  virtual HWND current_topmost_hwnd() const = 0;
};

class MOUSE_GESTURE_EXPORT MouseEventHooker {
public:
  explicit MouseEventHooker(MouseEventHookerDelegate* delegate);
  virtual ~MouseEventHooker();

protected:
  void Init();
  void CleanUp();

  friend LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);

  MouseEventHookerDelegate* delegate_;
};

NS_MG_END

#endif

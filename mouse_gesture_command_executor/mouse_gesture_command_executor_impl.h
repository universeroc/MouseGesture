// Copyright (c) 2013 universeroc. All rights reserved.
//
// universeroc@gmail.com
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __MOUSE_GESTURE_COMMAND_EXECUTOR_IMPL_H__
#define __MOUSE_GESTURE_COMMAND_EXECUTOR_IMPL_H__

#include "mouse_gesture_command_executor.h"

#include <Windows.h>

NS_MG_START

class MOUSE_GESTURE_EXPORT MouseGestureCommandExecutorImpl :
    public MouseGestureCommandExecutor {
public:
  MouseGestureCommandExecutorImpl(MouseGestureHandler* handler)
      : MouseGestureCommandExecutor(handler) {}
  
protected:
  virtual void ActiveNextWindow();

  virtual void ActivePrevWindow();

  virtual void CloseCurrentWindow();

  virtual void OpenLastClosedWindow();

  virtual void OpenApp(const wchar_t* app, const wchar_t* commandline);

  virtual void RestoreMessage();

  AppData GetAppDataFromHWND(HWND hwnd);

  bool DosPathToNtPath(wchar_t* pszDosPath, wchar_t* pszNtPath);
};

NS_MG_END

#endif

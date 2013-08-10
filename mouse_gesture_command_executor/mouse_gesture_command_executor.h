// Copyright (c) 2013 universeroc. All rights reserved.
//
// universeroc@gmail.com
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __MOUSE_GESTURE_COMMAND_EXECUTOR_H__
#define __MOUSE_GESTURE_COMMAND_EXECUTOR_H__

#include "../base/direction.h"
#include "../base/namespace.h"
#include "../mouse_gesture_data/mouse_gesture_data.h"

#ifndef _USRDLL
#define MOUSE_GESTURE_EXPORT __declspec(dllimport)
#else
#define MOUSE_GESTURE_EXPORT __declspec(dllexport)
#endif

NS_MG_START
class MouseGestureHandler;

class MOUSE_GESTURE_EXPORT MouseGestureCommandExecutor {
public:
  MouseGestureCommandExecutor(MouseGestureHandler* handler)
      : handler_(handler) {}

  void DispatchDirectionToExecutor(Direction direction);

protected:
  virtual void ActiveNextWindow() = 0;

  virtual void ActivePrevWindow() = 0;

  virtual void CloseCurrentWindow() = 0;

  virtual void OpenLastClosedWindow() = 0;

  virtual void OpenApp(const wchar_t* app, const wchar_t* commandline) = 0;

  virtual void RestoreMessage() = 0;

  MouseGestureData data_;
  MouseGestureHandler* handler_;
};

NS_MG_END

#endif

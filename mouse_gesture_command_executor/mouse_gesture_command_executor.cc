// Copyright (c) 2013 universeroc. All rights reserved.
//
// universeroc@gmail.com
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "mouse_gesture_command_executor.h"
#include <Windows.h>

#ifndef LOG
#define LOG(A) OutputDebugString(A)
#endif

namespace {

using namespace mouse_gesture;

struct DirectionLog {
  Direction dir;
  const wchar_t* dir_str;
};

DirectionLog direction_log[] = {
  { UP, L"UP" },
  { DOWN, L"DOWN" },
  { LEFT, L"LEFT" },
  { RIGHT, L"RIGHT" },
  { UP_LEFT, L"UP_LEFT" },
  { UP_RIGHT, L"UP_RIGHT" },
  { UP_DOWN, L"UP_DOWN" },
  { DOWN_UP, L"DOWN_UP" },
  { LEFT_UP, L"LEFT_UP" },
  { RIGHT_UP, L"RIGHT_UP" },
  { LEFT_DOWN, L"LEFT_DOWN" },
  { LEFT_RIGHT, L"LEFT_RIGHT" },
  { RIGHT_DOWN, L"RIGHT_DOWN" },
  { RIGHT_LEFT, L"RIGHT_LEFT" },
  { DOWN_LEFT, L"DOWN_LEFT" },
  { DOWN_RIGHT, L"DOWN_RIGHT" },
  { UNKNOWN, L"UNKOWN" },

  { a, L"a" },
  { B, L"B" },
  { C, L"C" },
  { D, L"D" },
  { e, L"e" },
  { G, L"G" },
  { h, L"h" },
  { N, L"N" },
  { q, L"q" },
  { I, L"I" },
};

unsigned int direction_log_size = sizeof(direction_log) / sizeof(DirectionLog);

}

NS_MG_START

void MouseGestureCommandExecutor::DispatchDirectionToExecutor(Direction direction) {
  for (int i = 0; i < direction_log_size; ++i) {
    if (direction_log[i].dir == direction)
      LOG(direction_log[i].dir_str);
  }

  std::vector<Data>::const_iterator iter = data_.data().cbegin();
  for (; iter != data_.data().cend(); ++iter) {
    if (static_cast<Direction>(iter->key) == direction) {
      Action action = static_cast<Action>(iter->value);
      switch (action) {
      case ACTIVE_NEXT_WINDOW:
        ActiveNextWindow();
        break;
      case ACTIVE_PREVIOUS_WINDOW:
        ActivePrevWindow();
        break;
      case CLOSE:
        CloseCurrentWindow();
        break;
      case OPEN_LAST_CLOSED_WINDOW:
        OpenLastClosedWindow();
        break;
      case OPEN:
        OpenApp(iter->appdata.app.c_str(), iter->appdata.commandline.c_str());
        break;
      case RESTORE_MESSAGE:
        RestoreMessage();
        break;
      }
    }
  }
}

NS_MG_END

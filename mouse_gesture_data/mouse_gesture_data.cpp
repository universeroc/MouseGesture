// Copyright (c) 2013 universeroc. All rights reserved.
//
// universeroc@gmail.com
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "mouse_gesture_data.h"
#include "../base/direction.h"
#include "../base/action.h"
#include <fstream>

namespace {
static const char kMouseGestureDataFileName[] = "MouseGesture";

using namespace mouse_gesture;

Data default_setting[] = {
  { UP, ACTIVE_PREVIOUS_WINDOW, L"", L"" },
  { DOWN, ACTIVE_NEXT_WINDOW, L"", L"" },
  { DOWN_RIGHT, CLOSE, L"", L"" },
  { DOWN_LEFT, OPEN_LAST_CLOSED_WINDOW, L"", L"" },
  { DOWN_UP, OPEN, L"devenv.exe", L"" },
  { a, OPEN, L"", L"" },
  { C, OPEN, L"chrome.exe", L"" },
  { e, OPEN, L"explorer.exe", L"" },
  { G, OPEN, L"www.google.com", L"" },
  { N, OPEN, L"notepad.exe", L"" },
  { q, OPEN, L"", L"" },
  { UNKNOWN, RESTORE_MESSAGE, L"", L""  }
};

}

NS_MG_START

MouseGestureData::MouseGestureData() {
  // first version we just use default setting
  unsigned int len = sizeof (default_setting) / sizeof(Data);
  for (int i = 0; i < len; ++i)
    data_.push_back(default_setting[i]);

  /*std::wfstream fs; //(kMouseGestureDataFileName, std::fstream::in | std::fstream::out | std::fstream::binary);
  fs.open(kMouseGestureDataFileName, std::fstream::out | std::fstream::binary);
  Data data;
  for (int i = 0; i < 10; i++) {
    data.key = i;
    // data.appdata.app = ('A' + i);
    data.value = i + 1;
    fs << data;
  }
  fs.close();

  std::wfstream fs1(kMouseGestureDataFileName, std::fstream::in | std::fstream::binary);
  
  while (!fs1.eof()) {
    Data data1;
    fs1 >> data1;
    OutputDebugString(data1.appdata.app.c_str());
  }*/
}

std::wostream& operator<<(std::wostream& os, const Data& data)  {
  os << data.key << data.value << std::endl;  // this line break is necessary.
  return os;
}

std::wistream& operator>>(std::wistream& is, Data& data) {
  is >> data.key >> data.value;
  // >> data.appdata.app >> data.appdata.commandline;
  return is;
}

NS_MG_END
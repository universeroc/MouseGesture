// Copyright (c) 2013 universeroc. All rights reserved.
//
// universeroc@gmail.com
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <windows.h>

// C RunTime Header Files
#include <tchar.h>
#include "mouse_gesture_handler.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

  mouse_gesture::MouseGestureHandler mouse_gesture_handler(hInstance);
  return mouse_gesture_handler.Run();
}

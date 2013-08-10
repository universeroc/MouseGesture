// Copyright (c) 2013 universeroc. All rights reserved.
//
// universeroc@gmail.com
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "mouse_event_hooker.h"

namespace {

#define LOG(A) OutputDebugString(A)

mouse_gesture::MouseEventHooker* g_mouse_event_process = NULL;
HHOOK g_mouse_ll = NULL;
}

NS_MG_START

MouseEventHooker::MouseEventHooker(MouseEventHookerDelegate* delegate)
    : delegate_(delegate) {
  Init();
}

MouseEventHooker::~MouseEventHooker() {
  CleanUp();
}

void MouseEventHooker::Init() {
  g_mouse_event_process = this;

  g_mouse_ll = ::SetWindowsHookEx(WH_MOUSE_LL, MouseProc, 0, 0);

  if (!g_mouse_ll)
    LOG(L"Fail to SetWindowHookEx!\n");
  else
    LOG(L"Success to SetWindowHookEx!\n");
}

void MouseEventHooker::CleanUp() {
  if (g_mouse_ll) {
    ::UnhookWindowsHookEx(g_mouse_ll);
    g_mouse_ll = NULL;
  }
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
  // If you alloc in stack some varible you would make the stack unbalance and get a corrupt.
  /*wchar_t* log = new wchar_t[20];
  memset(log, L'\0', 20);
  wsprintf(log, L"nCode: %d, wparam: %d\n", nCode, wParam);
  LOG(log);*/

  MSLLHOOKSTRUCT* ms = (MSLLHOOKSTRUCT*)lParam;
  if (g_mouse_event_process->delegate_->OnMouseEvent(wParam, *ms)) {
    return 1;
  }
  return ::CallNextHookEx(g_mouse_ll, nCode, wParam, lParam);
}

NS_MG_END

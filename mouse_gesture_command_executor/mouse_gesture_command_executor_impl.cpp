// Copyright (c) 2013 universeroc. All rights reserved.
//
// universeroc@gmail.com
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "mouse_gesture_command_executor_impl.h"
#include "../mouse_gesture/mouse_gesture_handler.h"

#include <Psapi.h>
#pragma comment (lib,"Psapi.lib")
#include <ShellAPI.h>

NS_MG_START

void MouseGestureCommandExecutorImpl::ActiveNextWindow() {
  HWND hwnd = handler_->current_topmost_hwnd();
  HWND next_hwnd = GetNextWindow(hwnd, GW_HWNDNEXT);
  ShowWindow(next_hwnd, SW_SHOWDEFAULT);
}

void MouseGestureCommandExecutorImpl::ActivePrevWindow() {
  HWND hwnd = handler_->current_topmost_hwnd();
  HWND next_hwnd = GetNextWindow(hwnd, GW_HWNDPREV);
  ShowWindow(next_hwnd, SW_SHOWDEFAULT);
}

void MouseGestureCommandExecutorImpl::CloseCurrentWindow() {
  HWND hwnd = handler_->current_topmost_hwnd();

  // we need log it so as to restore the window.
  AppData data = GetAppDataFromHWND(hwnd);
  handler_->data().closed_app_data().push_back(data);
  PostMessage(hwnd, WM_CLOSE, 0, 0);
}

void MouseGestureCommandExecutorImpl::OpenLastClosedWindow() {
  if (handler_->data().closed_app_data().empty())
    return;

  AppData data = handler_->data().closed_app_data().back();

  // app_name could be a url, than it open it with default browser.
  ShellExecute(NULL, L"open", data.app.c_str(), data.commandline.c_str(), 0, 0);

  handler_->data().closed_app_data().pop_back();
}

void MouseGestureCommandExecutorImpl::OpenApp(const wchar_t* app,
                                              const wchar_t* commandline) {
  ShellExecute(NULL, L"open", app, commandline, 0, 0);
}

void MouseGestureCommandExecutorImpl::RestoreMessage() {
  HWND hwnd = handler_->current_hwnd();
  POINT pt = handler_->path().back();
  MapWindowPoints(HWND_DESKTOP, hwnd, &pt, 1);
  PostMessage(hwnd, WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(pt.x, pt.y));
  PostMessage(hwnd, WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(pt.x, pt.y));
}

AppData MouseGestureCommandExecutorImpl::GetAppDataFromHWND(HWND hwnd) {
  AppData data;

  DWORD process_id = 0;
  GetWindowThreadProcessId(hwnd, &process_id);

  DWORD error = 0;
  HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, process_id);

  wchar_t path[MAX_PATH + 1] = {0};
  wchar_t nt_path[MAX_PATH + 1] = {0};
  if (!GetProcessImageFileName(handle, path, MAX_PATH)) {    
    CloseHandle(handle);
    return data;
  }

  if (!DosPathToNtPath(path, nt_path)) {
    CloseHandle(handle);
    return data;
  }

  data.app = nt_path;
  CloseHandle(handle);
  return data;
}

bool MouseGestureCommandExecutorImpl::DosPathToNtPath(wchar_t* pszDosPath, wchar_t* pszNtPath) {
  wchar_t           szDriveStr[500];  
  wchar_t           szDrive[3];  
  wchar_t           szDevName[100];  
  INT               cchDevName;  
  INT               i;  

  if(!pszDosPath || !pszNtPath )  
    return false;  

  if(GetLogicalDriveStrings(sizeof(szDriveStr), szDriveStr)) {  
    for(i = 0; szDriveStr[i]; i += 4) {  
      if(!lstrcmpi(&(szDriveStr[i]), L"A:\\") || !lstrcmpi(&(szDriveStr[i]), L"B:\\"))  
        continue;

      szDrive[0] = szDriveStr[i];  
      szDrive[1] = szDriveStr[i + 1];  
      szDrive[2] = '\0';  
      if(!QueryDosDevice(szDrive, szDevName, 100))
        return false;  

      cchDevName = lstrlen(szDevName);
      if(wcsncmp(pszDosPath, szDevName, cchDevName) == 0) {  
        lstrcpy(pszNtPath, szDrive);
        lstrcat(pszNtPath, pszDosPath + cchDevName);
        return true;  
      }
    }
  }

  lstrcpy(pszNtPath, pszDosPath);  

  return false;  
}  

NS_MG_END

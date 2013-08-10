// Copyright (c) 2013 universeroc. All rights reserved.
//
// universeroc@gmail.com
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __BASE_DATA_H__
#define __BASE_DATA_H__

#include <string>

#include "action.h"
#include "namespace.h"

NS_MG_START

struct AppData {
  std::wstring app;
  std::wstring commandline;
};

struct Data {
  unsigned long long key;       // enum Direction value
  // Action value;  enum is not a type could use << and >> to operate on.
  unsigned long long value;       // enum Action value
  AppData appdata;
};



NS_MG_END

#endif

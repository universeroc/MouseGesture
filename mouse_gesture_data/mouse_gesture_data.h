// Copyright (c) 2013 universeroc. All rights reserved.
//
// universeroc@gmail.com
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __MOUSE_GESTURE_DATA_H__
#define __MOUSE_GESTURE_DATA_H__

#include <iostream>
#include <vector>
#include "../base/data.h"
#include "../base/namespace.h"

#ifndef _USRDLL
#define MOUSE_GESTURE_EXPORT __declspec(dllimport)
#else
#define MOUSE_GESTURE_EXPORT __declspec(dllexport)
#endif

NS_MG_START

class MOUSE_GESTURE_EXPORT MouseGestureData {
public:
  MouseGestureData();

  const std::vector<Data>& data() const { return data_; }
  std::vector<AppData>& closed_app_data() { return closed_app_data_; }

private:
  friend std::wostream& operator<< (std::wostream& os, const Data& data);

  friend std::wistream& operator>> (std::wistream& is, Data& data);

  std::vector<Data> data_;
  std::vector<AppData> closed_app_data_;
};

NS_MG_END

#endif

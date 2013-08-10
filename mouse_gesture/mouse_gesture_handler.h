// Copyright (c) 2013 universeroc. All rights reserved.
//
// universeroc@gmail.com
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __MOUSE_GESTURE_HANDLER_H_
#define __MOUSE_GESTURE_HANDLER_H_

#include <vector>
#include <math.h>
#include "../base/namespace.h"
#include "../base/direction.h"
#include "../mouse_event_hooker/mouse_event_hooker.h"
#include "../mouse_gesture_data/mouse_gesture_data.h"

NS_MG_START

class MouseGestureView;
class MouseGestureCommandExecutor;

class MouseGestureHandler : public MouseEventHookerDelegate {
public:
  MouseGestureHandler(HINSTANCE instance);
  virtual ~MouseGestureHandler(void);

  int Run();

  const std::vector<POINT>& path() const { return path_; }

  // overriden from mouse_gesture::MouseEventHookerDelegate
  virtual bool OnMouseEvent(UINT event, const MSLLHOOKSTRUCT& ms);
  virtual HWND current_hwnd() const { return current_hwnd_; }
  virtual HWND current_topmost_hwnd() const { return current_topmost_hwnd_; }

  MouseGestureData& data() { return data_; }

protected:
  HWND GetNativeWindow();
  void OnRButtonDown(const POINT& pt);
  void OnMouseMove(const POINT& pt);
  void OnRButtonUp(const POINT& pt);

  class Recognizor {
  public:
    Recognizor() {
      prev_point_.x = -1;
      prev_point_.y = -1;
    }
    virtual ~Recognizor() {}

    void FromPoint(const POINT& pt) {
      prev_point_ = pt;
      direction_ = UNKNOWN;
    }

    Direction direction() const { return direction_; }

    Direction OnRecognize(const POINT& pt) {
      UINT64 distance_square = (pt.x - prev_point_.x) * (pt.x - prev_point_.x) +
          (pt.y - prev_point_.y) * (pt.y - prev_point_.y);
      if (distance_square < kRecognizeAccuracyPx)
        return direction_;

      LONG delta_x = pt.x - prev_point_.x;
      LONG delta_y = pt.y - prev_point_.y;

      if (abs(delta_y) > abs(delta_x) && delta_y > 0) {
        // DOWN
        if (direction_ & DOWN ^ DOWN)
          direction_ = (Direction)MAKEDIRECTION(direction_, DOWN, 4);
      } else if (abs(delta_y) > abs(delta_x) && delta_y < 0) {
        // UP
        if (direction_ & UP ^ UP)
          direction_ = (Direction)MAKEDIRECTION(direction_, UP, 4);
      } else if (abs(delta_y) < abs(delta_x) && delta_x > 0) {
        // RIGHT
        if (direction_ & RIGHT ^ RIGHT)
          direction_ = (Direction)MAKEDIRECTION(direction_, RIGHT, 4);
      } else if (abs(delta_y) < abs(delta_x) && delta_x < 0) {
        // LEFT
        if (direction_ & LEFT ^ LEFT)
          direction_ = (Direction)MAKEDIRECTION(direction_, LEFT, 4);
      }

      prev_point_ = pt;
      return direction_;
    }

  protected:
    static const UINT64 kRecognizeAccuracyPx = 16;

    Direction direction_;
    POINT prev_point_;
  };

  Recognizor recognizor_;

  MouseGestureView* view_;
  MouseEventHooker* event_hooker_;
  MouseGestureCommandExecutor* command_executor_;
  MouseGestureData data_;

  HWND current_hwnd_;
  HWND current_topmost_hwnd_;
  
  bool right_button_down_;
  std::vector<POINT> path_;
};

NS_MG_END // namespace mouse_gesture

#endif

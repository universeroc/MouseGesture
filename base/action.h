// Copyright (c) 2013 universeroc. All rights reserved.
//
// universeroc@gmail.com
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __BASE_ACTION_H__
#define __BASE_ACTION_H__

#include "namespace.h"

NS_MG_START

enum Action {
  ACTIVE_PREVIOUS_WINDOW,
  ACTIVE_NEXT_WINDOW,
  CLOSE,
  OPEN_LAST_CLOSED_WINDOW,
  OPEN,
  RESTORE_MESSAGE,
};

NS_MG_END

#endif

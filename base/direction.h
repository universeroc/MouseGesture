// Copyright (c) 2013 universeroc. All rights reserved.
//
// universeroc@gmail.com
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __BASE_DIRECTION_H__
#define __BASE_DIRECTION_H__

#include "namespace.h"

namespace {

#define MAKEDIRECTION(A, B, N)  \
  (((A) << N) | (B))

#define MAKEDIRECTION2(A, B, C, N) \
  MAKEDIRECTION(MAKEDIRECTION(A, B, N), C, N)

#define MAKEDIRECTION3(A, B, C, D, N) \
  MAKEDIRECTION(MAKEDIRECTION2(A, B, C, N), D, N)

#define MAKEDIRECTION4(A, B, C, D, E, N) \
  MAKEDIRECTION(MAKEDIRECTION3(A, B, C, D, N), E, N)

#define MAKEDIRECTION5(A, B, C, D, E, F, N)  \
  MAKEDIRECTION(MAKEDIRECTION4(A, B, C, D, E, N), F, N)

#define MAKEDIRECTION6(A, B, C, D, E, F, G, N) \
  MAKEDIRECTION(MAKEDIRECTION5(A, B, C, D, E, F, N), G, N)

#define MAKEDIRECTION7(A, B, C, D, E, F, G, H, N) \
  MAKEDIRECTION(MAKEDIRECTION6(A, B, C, D, E, F, G, N), H, N)

#define MAKEDIRECTION8(A, B, C, D, E, F, G, H, I, N) \
  MAKEDIRECTION(MAKEDIRECTION7(A, B, C, D, E, F, G, H, N), I, N)
}

NS_MG_START

enum Direction {
  UNKNOWN       = 0,
  LEFT          = 1,
  RIGHT         = 2,
  UP            = 4,
  DOWN          = 8,

  // Two basic directions combined
  LEFT_RIGHT    = MAKEDIRECTION(LEFT, RIGHT, 4),
  LEFT_UP       = MAKEDIRECTION(LEFT, UP, 4),
  LEFT_DOWN     = MAKEDIRECTION(LEFT, DOWN, 4),

  RIGHT_LEFT    = MAKEDIRECTION(RIGHT, LEFT, 4),
  RIGHT_UP      = MAKEDIRECTION(RIGHT, UP, 4),
  RIGHT_DOWN    = MAKEDIRECTION(RIGHT, DOWN, 4),

  UP_LEFT       = MAKEDIRECTION(UP, LEFT, 4),
  UP_RIGHT      = MAKEDIRECTION(UP, RIGHT, 4),
  UP_DOWN       = MAKEDIRECTION(UP, DOWN, 4),

  DOWN_LEFT     = MAKEDIRECTION(DOWN, LEFT, 4),
  DOWN_RIGHT    = MAKEDIRECTION(DOWN, RIGHT, 4),
  DOWN_UP       = MAKEDIRECTION(DOWN, UP, 4),

  // letter directions
  a             = MAKEDIRECTION4(LEFT, DOWN, UP, DOWN, RIGHT, 4),
  B             = MAKEDIRECTION5(DOWN, UP, RIGHT, LEFT, RIGHT, LEFT, 4),
  C             = MAKEDIRECTION2(LEFT, DOWN, RIGHT, 4),
  D             = MAKEDIRECTION4(DOWN, UP, RIGHT, DOWN, LEFT, 4),
  e             = MAKEDIRECTION4(RIGHT, UP, LEFT, DOWN, RIGHT, 4),
  f             = MAKEDIRECTION3(LEFT, DOWN, UP, RIGHT, 4),
  G             = MAKEDIRECTION4(LEFT, DOWN, RIGHT, UP, DOWN, 4),
  h             = MAKEDIRECTION4(DOWN, UP, RIGHT, DOWN, RIGHT, 4),
  // l             = MAKEDIRECTION(DOWN, RIGHT, 4), same as DOWN_RIGHT
  N             = MAKEDIRECTION3(DOWN, UP, DOWN, UP, 4),
  q             = MAKEDIRECTION4(LEFT, DOWN, RIGHT, UP, DOWN, 4),
  I
};

NS_MG_END

#endif


#pragma once

namespace invaderz::mouse {

enum Button
{
  UNKNOWN = 0,
  LEFT,
  MIDDLE,
  RIGHT,

  BUTTONS_COUNT
};

enum State
{
  FREE = 0,
  RELEASED,
  PRESSED,
  HELD
};

} // namespace invaderz::mouse

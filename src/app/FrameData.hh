
#pragma once

#include "InputState.hh"

namespace invaderz {

struct FrameData
{
  bool quit{false};
  InputState state{};

  /// @param elapsed - the number of seconds elapsed in the last frame
  float elapsed{};
};

} // namespace invaderz

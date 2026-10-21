
#pragma once

#include <string>

namespace invaderz {

enum class Screen
{
  WELCOME,
  REGULAR_LEVEL,
  BOSS_LEVEL,
  PAUSE,
  END_GAME,
  EXIT
};

auto str(const Screen screen) -> std::string;

} // namespace invaderz

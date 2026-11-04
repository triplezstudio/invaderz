
#pragma once

#include <string>

namespace invaderz {

enum class Screen
{
  WELCOME,
  LEVEL,
  END_GAME,
  EXIT
};

auto str(const Screen screen) -> std::string;

} // namespace invaderz


#pragma once

#include <string>

namespace invaderz {

enum class Subscreen
{
  LOAD,
  RUN,
  PAUSE
};

auto str(const Subscreen subscreen) -> std::string;

} // namespace invaderz

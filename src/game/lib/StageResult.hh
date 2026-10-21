
#pragma once

#include "Screen.hh"
#include <optional>

namespace invaderz {

struct StageResult
{
  bool finished{};
  std::optional<Screen> next{};
};

} // namespace invaderz
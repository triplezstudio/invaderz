
#pragma once

#include <SDL3/SDL.h>
#include <vector>

namespace invaderz {

struct EventData
{
  std::vector<SDL_Event> events{};
};

} // namespace invaderz

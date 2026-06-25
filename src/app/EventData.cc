
#include "EventData.hh"

namespace invaderz {

bool EventData::quitRequested() const
{
  for (const auto &event : events)
  {
    if (event.type == SDL_EVENT_QUIT)
    {
      return true;
    }
    if (event.type == SDL_EVENT_KEY_UP && event.key.key == SDLK_ESCAPE)
    {
      return true;
    }
  }

  return false;
}

} // namespace invaderz

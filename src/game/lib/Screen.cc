
#include "Screen.hh"

namespace invaderz {

auto str(const Screen screen) -> std::string
{
  switch (screen)
  {
    case Screen::WELCOME:
      return "welcome";
    case Screen::REGULAR_LEVEL:
      return "regular_level";
    case Screen::BOSS_LEVEL:
      return "boss_level";
    case Screen::PAUSE:
      return "pause";
    case Screen::END_GAME:
      return "end_game";
    case Screen::EXIT:
      return "exit";
    default:
      return "unknown";
  };
}

} // namespace invaderz

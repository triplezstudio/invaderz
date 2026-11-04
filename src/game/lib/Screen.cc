
#include "Screen.hh"

namespace invaderz {

auto str(const Screen screen) -> std::string
{
  switch (screen)
  {
    case Screen::WELCOME:
      return "welcome";
    case Screen::LEVEL:
      return "level";
    case Screen::BOSS_LEVEL:
      return "boss_level";
    case Screen::END_GAME:
      return "end_game";
    case Screen::EXIT:
      return "exit";
    default:
      return "unknown";
  };
}

} // namespace invaderz

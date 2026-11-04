
#include "Screen.hh"

namespace invaderz {

auto str(const Screen screen) -> std::string
{
  switch (screen)
  {
    case Screen::WELCOME:
      return "welcome";
    case Screen::END_GAME:
      return "end_game";
    case Screen::EXIT:
      return "exit";
    default:
      return "unknown";
  };
}

} // namespace invaderz

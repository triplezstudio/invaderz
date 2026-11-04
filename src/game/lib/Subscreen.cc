
#include "Subscreen.hh"

namespace invaderz {

auto str(const Subscreen subscreen) -> std::string
{
  switch (subscreen)
  {
    case Subscreen::LOAD:
      return "load";
    case Subscreen::RUN:
      return "run";
    case Subscreen::PAUSE:
      return "pause";
    default:
      return "unknown";
  };
}

} // namespace invaderz

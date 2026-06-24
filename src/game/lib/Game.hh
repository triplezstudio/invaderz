
#pragma once

#include "CoreObject.hh"

namespace invaderz {

class Game : public runtime::CoreObject
{
  public:
  Game();
  ~Game() override = default;

  void update();
};

} // namespace invaderz

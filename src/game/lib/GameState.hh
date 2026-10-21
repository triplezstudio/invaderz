
#pragma once

namespace invaderz {

struct GameState
{
  int lives{15};
  int score{0};

  void reset();
};

} // namespace invaderz

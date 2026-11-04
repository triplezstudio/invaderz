
#pragma once

namespace invaderz {

struct GameState
{
  int lives{15};
  int score{0};
  int level{0};
  int bossLevelInterval{3};
  int maxLevel{6};

  void reset();
};

} // namespace invaderz

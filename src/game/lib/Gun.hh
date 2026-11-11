
#pragma once

namespace invaderz {

struct Gun
{
  float firingInterval{};
  float sinceLastFire{0.0f};
  int damage{};
};

} // namespace invaderz

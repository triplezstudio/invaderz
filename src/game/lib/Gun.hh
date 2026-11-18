
#pragma once

namespace invaderz {

enum class FiringStrategy
{
  CONSTANT,
  JITTERED,
};

struct Gun
{
  FiringStrategy strategy{FiringStrategy::CONSTANT};
  float baseFiringInterval{};
  float nextFiringInterval{};
  float sinceLastFire{0.0f};
  int damage{};

  void prepareForNextShot();
};

auto createLevelBasedGun(const int levelMultiplier) -> Gun;

} // namespace invaderz

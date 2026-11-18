
#pragma once

#include "Bullet.hh"

namespace invaderz {

enum class FiringStrategy
{
  CONSTANT,
  JITTERED,
};

enum class FiringPattern
{
  SINGLE,
  BAR,
  STAR,
  SPRAY,
};

struct Gun
{
  FiringPattern pattern{FiringPattern::SINGLE};
  FiringStrategy strategy{FiringStrategy::CONSTANT};
  float baseFiringInterval{};
  float nextFiringInterval{};
  float sinceLastFire{0.0f};
  int damage{};
  float bulletSpeed{};

  auto tryFire(const float elapsed, const Eigen::Vector3f position) -> std::vector<Bullet>;

  private:
  void prepareForNextShot();
  auto fire(const Eigen::Vector3f position) -> std::vector<Bullet>;
};

auto createLevelBasedGun(const int levelMultiplier, const float bulletSpeed) -> Gun;

} // namespace invaderz

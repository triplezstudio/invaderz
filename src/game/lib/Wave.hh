
#pragma once

#include "Enemy.hh"
#include <vector>

namespace invaderz {

struct Wave
{
  std::vector<Enemy> enemies{};

  Wave(const Eigen::Vector3f &worldDims);
  ~Wave() = default;

  void move(const float elapsed, const Eigen::Vector3f &worldDims);
  void cleanInvadingEnemies();
};

} // namespace invaderz

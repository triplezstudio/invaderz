
#pragma once

#include <eigen3/Eigen/Eigen>

namespace invaderz {

struct Bullet
{
  int damage{};
  Eigen::Vector3f pos{};
  bool canDamagePlayer{false};
};

} // namespace invaderz

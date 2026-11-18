
#pragma once

#include <eigen3/Eigen/Eigen>

namespace invaderz {

struct Bullet
{
  int damage{};
  Eigen::Vector3f pos{};
  Eigen::Vector3f speed{};
  bool canDamagePlayer{false};
};

} // namespace invaderz

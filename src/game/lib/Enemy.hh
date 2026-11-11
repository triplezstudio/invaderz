
#pragma once

#include "Gun.hh"
#include <eigen3/Eigen/Eigen>

namespace invaderz {

struct Enemy
{
  float xInit{};
  int worth{};
  int health{};
  Eigen::Vector3f pos{};
  Gun gun{};
};

} // namespace invaderz

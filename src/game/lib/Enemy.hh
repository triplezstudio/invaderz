
#pragma once

#include <eigen3/Eigen/Eigen>

namespace invaderz {

struct Enemy
{
  float xInit{};
  int worth{};
  int health{};
  Eigen::Vector3f pos{};
};

} // namespace invaderz

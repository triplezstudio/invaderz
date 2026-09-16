
#pragma once

#include <eigen3/Eigen/Eigen>
#include <vector>

namespace invaderz {

struct Effects
{
  std::vector<Eigen::Vector3f> explosions{};
};

} // namespace invaderz


#pragma once

#include "Wave.hh"
#include <eigen3/Eigen/Eigen>
#include <vector>

namespace invaderz {

struct Level
{
  int lives{};
  Eigen::Vector3f dimensions{};
  std::vector<Wave> waves{};

  Level(const int lives, const int wavesCount, const Eigen::Vector3f &worldDims);
  ~Level() = default;
};

} // namespace invaderz

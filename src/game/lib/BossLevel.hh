
#pragma once

#include "Wave.hh"
#include <eigen3/Eigen/Eigen>
#include <vector>

namespace invaderz {

struct BossLevel
{
  Eigen::Vector3f dimensions{};
  std::vector<Wave> waves{};

  BossLevel(const int wavesCount, const Eigen::Vector3f &worldDims);
  ~BossLevel() = default;
};

} // namespace invaderz

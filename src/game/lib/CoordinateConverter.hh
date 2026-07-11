
#pragma once

#include <eigen3/Eigen/Eigen>

namespace invaderz {

class CoordinateConverter
{
  public:
  CoordinateConverter(const Eigen::Vector3f &worldDims, const Eigen::Vector3f &screenDims);
  ~CoordinateConverter() = default;

  auto toScreenPos(const Eigen::Vector3f &worldPos, const Eigen::Vector3f &size) const
    -> Eigen::Vector3f;

  private:
  Eigen::Vector3f m_worldDims{};
  Eigen::Vector3f m_screenDims{};

  Eigen::Vector3f m_offset{};
};

} // namespace invaderz

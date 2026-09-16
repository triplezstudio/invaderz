
#include "CoordinateConverter.hh"

#include "VectorUtils.hh"
#include <iostream>

namespace invaderz {

CoordinateConverter::CoordinateConverter(const Eigen::Vector3f &worldDims,
                                         const Eigen::Vector3f &screenOffset,
                                         const Eigen::Vector3f &displayDims)
  : m_worldDims(worldDims)
  , m_screenOffset(screenOffset)
  , m_displayDims(displayDims)
{}

auto CoordinateConverter::toScreenPos(const Eigen::Vector3f &worldPos,
                                      const Eigen::Vector3f &size) const -> Eigen::Vector3f
{
  const auto xScreen = m_screenOffset(0) + (worldPos(0) / m_worldDims(0)) * m_displayDims(0);
  const auto yScreen = m_screenOffset(1)
                       + (1.0f - (worldPos(1) / m_worldDims(1))) * m_displayDims(1);

  return Eigen::Vector3f(xScreen - size(0) / 2.0f, yScreen - size(1) / 2.0f, 0.0f);
}

} // namespace invaderz

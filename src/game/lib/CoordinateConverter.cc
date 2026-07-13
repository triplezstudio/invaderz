
#include "CoordinateConverter.hh"

#include "VectorUtils.hh"
#include <iostream>

namespace invaderz {

CoordinateConverter::CoordinateConverter(const Eigen::Vector3f &worldDims,
                                         const Eigen::Vector3f &screenDims)
  : m_worldDims(worldDims)
  , m_screenDims(screenDims)
  , m_offset((m_screenDims - m_worldDims) / 2.0f)
{}

auto CoordinateConverter::toScreenPos(const Eigen::Vector3f &worldPos,
                                      const Eigen::Vector3f &size) const -> Eigen::Vector3f
{
  const auto rawScreenPos = worldPos + m_offset;
  const auto x            = rawScreenPos(0) - size(0) / 2.0f;
  const auto y            = m_screenDims(1) - rawScreenPos(1) - size(1) / 2.0f;
  return Eigen::Vector3f(x, y, 0.0f);
}

} // namespace invaderz

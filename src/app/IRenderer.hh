
#pragma once

#include "Color.hh"
#include <eigen3/Eigen/Eigen>

namespace invaderz {

class IRenderer
{
  public:
  IRenderer()          = default;
  virtual ~IRenderer() = default;

  virtual void renderRectangle(const Eigen::Vector3f &position,
                               const Eigen::Vector3f &dims,
                               const Color color)
    = 0;
};

} // namespace invaderz


#pragma once

#include <eigen3/Eigen/Eigen>

namespace invaderz {

class IRenderer
{
  public:
  IRenderer()          = default;
  virtual ~IRenderer() = default;

  virtual void renderRectangle(const Eigen::Vector3f &position, const Eigen::Vector3f &dims) = 0;
};

} // namespace invaderz

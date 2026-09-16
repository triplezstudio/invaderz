
#pragma once

#include "IRenderer.hh"
#include <cstdint>
#include <eigen3/Eigen/Eigen>

namespace invaderz {

struct Explosion
{
  Eigen::Vector3f position{};
  float remainingDuration{0.0f};
  std::size_t index{0};

  Explosion(Eigen::Vector3f inPosition);

  bool finished() const;

  void update(float elapsed);
  void render(IRenderer &renderer, const TextureId texture);
};

} // namespace invaderz

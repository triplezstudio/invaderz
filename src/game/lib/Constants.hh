
#pragma once

#include <eigen3/Eigen/Eigen>

namespace invaderz {

// The dimensions are expressed in pixels.
auto playerDimensions() -> Eigen::Vector3f;
auto enemyDimensions() -> Eigen::Vector3f;
auto bulletDimensions() -> Eigen::Vector3f;
auto explosionDimensions() -> Eigen::Vector3f;

} // namespace invaderz

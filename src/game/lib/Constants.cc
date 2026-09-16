
#include "Constants.hh"

namespace invaderz {
namespace {
const Eigen::Vector3f PLAYER_DIMS(64.0f, 64.0f, 0.0f);
const Eigen::Vector3f ENEMY_DIMS(32.0f, 32.0f, 0.0f);
const Eigen::Vector3f BULLET_DIMS(16.0f, 16.0f, 0.0f);
} // namespace

auto playerDimensions() -> Eigen::Vector3f
{
  return PLAYER_DIMS;
}

auto enemyDimensions() -> Eigen::Vector3f
{
  return ENEMY_DIMS;
}

auto bulletDimensions() -> Eigen::Vector3f
{
  return BULLET_DIMS;
}

auto explosionDimensions() -> Eigen::Vector3f
{
  return 1.6f * enemyDimensions();
}

} // namespace invaderz

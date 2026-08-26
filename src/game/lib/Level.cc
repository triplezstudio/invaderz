
#include "Level.hh"

namespace invaderz {
namespace {
constexpr float SPACE_BETWEEN_WAVES = 80.0f;
}

Level::Level(const int lives, const int wavesCount, const Eigen::Vector3f &worldDims)
  : lives(lives)
  , dimensions(worldDims)
{
  for (int id = 0; id < wavesCount; ++id)
  {
    waves.emplace_back(dimensions, id * SPACE_BETWEEN_WAVES);
  }
}

} // namespace invaderz


#include "Explosion.hh"
#include "Constants.hh"

namespace invaderz {
namespace {
constexpr std::size_t FRAME_COUNT = 10;
const Eigen::Vector3f FRAME_SIZE(103.0f, 103.0f, 0.0f);

constexpr auto STEP_DURATION = 0.05f;
} // namespace

Explosion::Explosion(Eigen::Vector3f inPosition)
  : position(std::move(inPosition))
{}

bool Explosion::finished() const
{
  return index > FRAME_COUNT;
}

void Explosion::update(float elapsed)
{
  remainingDuration -= elapsed;

  if (remainingDuration < 0.0f)
  {
    remainingDuration = STEP_DURATION;
    ++index;
  }
}

void Explosion::render(IRenderer &renderer, const TextureId texture)
{
  Eigen::Vector3f src(index * FRAME_SIZE(0), 0.0f, 0.0f);
  Eigen::Vector3f dstDims = 1.6f * enemyDimensions();
  renderer.renderPartialTexture(texture, src, FRAME_SIZE, position, dstDims);
}

} // namespace invaderz

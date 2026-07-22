
#include "Wave.hh"

namespace invaderz {
namespace {
// The speeds are expressed in pixels per second
constexpr auto ENEMY_SPEED = 20;

// Defines the number of horizontal oscillation each enemy will do over the course of
// its journey towards the bottom of the screen.
constexpr auto OSCILLATION_COUNT = 5;

// Defines the amplitude of an horizontal oscillation of the enemies in pixels.
constexpr auto OSCILLATION_AMPLITUDE = 30;

constexpr auto ENEMY_WAVE_COUNT = 5;

// This is the percentage of the width of the screen that the enemy
// wave spans.
constexpr auto ENEMY_WAVE_PERCENTAGE = 0.9f;

auto calculateXOffset(const float y, const Eigen::Vector3f dims) -> float
{
  // The world has `dims` for dimensions. Each enemy should complete `OSCILLATION_COUNT` oscillations
  // over its journey towards the bottom.
  // To jitter its position along the `x` axis, the cosine function is used. Cosine has a period of
  // 2 pi.
  // It is possible to map the size of the world to the interval `OSCILLATION_COUNT * 2 * pi` so that
  // cosine completes the right amount of periods.
  // It is possible to map the current `y` position of the enemy to a percentage in the interval
  // [0; 1] to calculate the current position in the cosine domain.
  // It looks like the following/
  //  - screen [0; dims(1)]
  //  - normalized [0; 1]
  //  - cosine space [0; OSCILLATION_COUNT * 2 * pi]
  //  - apply cosine to get the oscillation
  const auto normalized  = y / dims(1);
  const auto cosineSpace = normalized * OSCILLATION_COUNT * 2 * std::numbers::pi_v<float>;
  return OSCILLATION_AMPLITUDE * std::cos(cosineSpace);
}
} // namespace

Wave::Wave(const Eigen::Vector3f &worldDims)
{
  const auto waveSpan = ENEMY_WAVE_PERCENTAGE * worldDims(0);
  const auto xStart   = (worldDims(0) - waveSpan) / 2.0f;
  const auto interval = waveSpan / (ENEMY_WAVE_COUNT - 1);

  for (int i = 0; i < ENEMY_WAVE_COUNT; ++i)
  {
    const auto x = xStart + i * interval;
    const auto y = worldDims(1);

    Enemy enemy{
      .xInit = x,
      .pos   = Eigen::Vector3f(x, y, 0.0f),
    };
    enemies.push_back(enemy);
  }
}

void Wave::move(const float elapsed, const Eigen::Vector3f &worldDims)
{
  for (auto &enemy : enemies)
  {
    const auto xOffset = calculateXOffset(enemy.pos(1), worldDims);
    enemy.pos(0)       = enemy.xInit + xOffset;
    enemy.pos(1) -= (elapsed * ENEMY_SPEED);
  }
}

void Wave::cleanInvadingEnemies()
{
  std::erase_if(enemies, [this](const Enemy &enemy) { return enemy.pos(1) < 0.0f; });
}

} // namespace invaderz

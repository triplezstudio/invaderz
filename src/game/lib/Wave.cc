
#include "Wave.hh"
#include <random>

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

constexpr auto ENEMY_BASE_HEALTH = 1;
constexpr auto ENEMY_BASE_WORTH  = 10;

auto calculateXOffset(const float y, const float xSpeed, const Eigen::Vector3f dims) -> float
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
  const auto cosineSpace = normalized * xSpeed * OSCILLATION_COUNT * 2 * std::numbers::pi_v<float>;
  return OSCILLATION_AMPLITUDE * std::cos(cosineSpace);
}
} // namespace

Wave::Wave(const Eigen::Vector3f &worldDims, const float yOffset, const int levelMultiplier)
{
  const auto waveSpan = ENEMY_WAVE_PERCENTAGE * worldDims(0);
  const auto xStart   = (worldDims(0) - waveSpan) / 2.0f;
  // const auto enemyCount = ENEMY_WAVE_COUNT + (levelMultiplier - 1);
  const auto enemyCount = ENEMY_WAVE_COUNT + levelMultiplier;
  const auto interval   = waveSpan / (enemyCount - 1);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> xJitter(-35.0f, 35.0f);
  std::uniform_real_distribution<float> yJitter(-25.0f, 25.0f);
  std::uniform_real_distribution<float> vxJitter(1.0f, 3.0f);
  std::uniform_real_distribution<float> vyJitter(-5.0f, 40.0f);

  for (int i = 0; i < enemyCount; ++i)
  {
    const auto x = xStart + i * interval;
    const auto y = worldDims(1) + yOffset + yJitter(gen);

    const auto vx = vxJitter(gen);
    const auto vy = ENEMY_SPEED + vyJitter(gen);

    Enemy enemy{
      .xInit  = x + xJitter(gen),
      .worth  = ENEMY_BASE_WORTH * (i + 1),
      .health = ENEMY_BASE_HEALTH * levelMultiplier,
      .speed  = Eigen::Vector3f(vx, vy, 0.0f),
      .pos    = Eigen::Vector3f(x, y, 0.0f),
      .gun    = createLevelBasedGun(levelMultiplier),
    };
    enemies.push_back(enemy);
  }
}

bool Wave::empty() const
{
  return enemies.empty();
}

void Wave::move(const float elapsed, const Eigen::Vector3f &worldDims)
{
  for (auto &enemy : enemies)
  {
    const auto xOffset = calculateXOffset(enemy.pos(1), enemy.speed(0), worldDims);
    enemy.pos(0)       = enemy.xInit + xOffset;
    enemy.pos(1) -= (elapsed * enemy.speed(1));
  }
}

void Wave::cleanInvadingEnemies()
{
  std::erase_if(enemies, [this](const Enemy &enemy) { return enemy.pos(1) < 0.0f; });
}

} // namespace invaderz

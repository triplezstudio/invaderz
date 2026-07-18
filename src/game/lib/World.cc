
#include "World.hh"
#include "VectorUtils.hh"

#include <iostream>

namespace invaderz {
namespace {
// The speeds are expressed in pixels per second
constexpr auto PLAYER_SPEED = 280;
constexpr auto BULLET_SPEED = 800;
constexpr auto ENEMY_SPEED  = 20;

// The interval is expressed in seconds
constexpr auto ENEMY_WAVE_INTERVAL = 3.0;

constexpr auto ENEMY_WAVE_COUNT = 5;
// This is the percentage of the width of the screen that the enemy
// wave spans.
constexpr auto ENEMY_WAVE_PERCENTAGE = 0.9f;
} // namespace

World::World(Eigen::Vector3f dims)
  : runtime::CoreObject("world")
  , m_dims(std::move(dims))
{
  initialize();
}

auto World::dims() const -> const Eigen::Vector3f &
{
  return m_dims;
}

auto World::playerPosition() const -> const Eigen::Vector3f &
{
  return m_player;
}

auto World::bullets() const -> const std::vector<Eigen::Vector3f> &
{
  return m_bullets;
}

auto World::enemies() const -> std::vector<Eigen::Vector3f>
{
  std::vector<Eigen::Vector3f> out{};
  std::transform(m_enemies.begin(),
                 m_enemies.end(),
                 std::back_inserter(out),
                 [](const Enemy &enemy) { return enemy.pos; });
  return out;
}

void World::movePlayer(const Eigen::Vector3f &motion)
{
  m_player += (motion * PLAYER_SPEED);
  if (m_player(0) < 0.0f)
  {
    m_player(0) = 0.0f;
  }
  if (m_player(0) > m_dims(0))
  {
    m_player(0) = m_dims(0);
  }
}

void World::fire()
{
  m_bullets.emplace_back(m_player);
}

void World::update(const float elapsed)
{
  Eigen::Vector3f motion(0, elapsed * BULLET_SPEED, 0.0f);

  for (auto &bullet : m_bullets)
  {
    bullet += motion;
  }

  removeOutOfBoundsBullets();
  maybeSpawnEnemyWave(elapsed);
  moveEnemies(elapsed);
  removeInvadingEnemies();
}

void World::initialize()
{
  m_player = Eigen::Vector3f(m_dims(0) / 2.0f, 0.0f, 0.0f);
  info("player pos " + str(m_player));
}

void World::maybeSpawnEnemyWave(const float elapsed)
{
  m_elapsedSinceLastEnemyWave += elapsed;

  if (m_elapsedSinceLastEnemyWave < ENEMY_WAVE_INTERVAL)
  {
    return;
  }

  m_elapsedSinceLastEnemyWave = 0.0f;

  const auto waveSpan = ENEMY_WAVE_PERCENTAGE * m_dims(0);
  const auto xStart   = (m_dims(0) - waveSpan) / 2.0f;
  const auto interval = waveSpan / (ENEMY_WAVE_COUNT - 1);

  for (int i = 0; i < ENEMY_WAVE_COUNT; ++i)
  {
    const auto x = xStart + i * interval;
    const auto y = m_dims(1);

    info("spawned enemy at " + str(Eigen::Vector3f(x, y, 0.0f)));

    Enemy enemy{
      .waveId = m_lastWaveId,
      .xInit  = x,
      .pos    = Eigen::Vector3f(x, y, 0.0f),
    };
    m_enemies.push_back(enemy);
  }

  ++m_lastWaveId;
}

namespace {
// Defines the number of horizontal oscillation each enemy will do over the course of
// its journey towards the bottom of the screen.
constexpr auto OSCILLATION_COUNT = 5;

// Defines the amplitude of an horizontal oscillation of the enemies in pixels.
constexpr auto OSCILLATION_AMPLITUDE = 30;

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

void World::moveEnemies(const float elapsed)
{
  for (auto &enemy : m_enemies)
  {
    const auto multiplier = enemy.waveId % 2 == 0 ? 1.0f : -1.0f;
    const auto xOffset    = multiplier * calculateXOffset(enemy.pos(1), m_dims);
    enemy.pos(0)          = enemy.xInit + xOffset;
    enemy.pos(1) -= (elapsed * ENEMY_SPEED);
  }
}

void World::removeOutOfBoundsBullets()
{
  std::erase_if(m_bullets, [this](const Eigen::Vector3f &bullet) { return bullet(1) > m_dims(1); });
}

void World::removeInvadingEnemies()
{
  std::erase_if(m_enemies, [this](const Enemy &enemy) { return enemy.pos(1) < 0.0f; });
}

} // namespace invaderz
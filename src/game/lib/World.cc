
#include "World.hh"
#include "VectorUtils.hh"

#include <iostream>

namespace invaderz {
namespace {
// The speeds are expressed in pixels per second
constexpr auto PLAYER_SPEED = 280;
constexpr auto BULLET_SPEED = 800;

// The interval is expressed in seconds
constexpr auto ENEMY_WAVE_INTERVAL = 3.0;
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

  for (const auto &wave : m_waves)
  {
    std::transform(wave.enemies.begin(),
                   wave.enemies.end(),
                   std::back_inserter(out),
                   [](const Enemy &enemy) { return enemy.pos; });
  }

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

  m_waves.emplace_back(m_dims);
}

void World::moveEnemies(const float elapsed)
{
  for (auto &wave : m_waves)
  {
    wave.move(elapsed, m_dims);
  }
}

void World::removeOutOfBoundsBullets()
{
  std::erase_if(m_bullets, [this](const Eigen::Vector3f &bullet) { return bullet(1) > m_dims(1); });
}

void World::removeInvadingEnemies()
{
  for (auto &wave : m_waves)
  {
    wave.cleanInvadingEnemies();
  }
}

} // namespace invaderz
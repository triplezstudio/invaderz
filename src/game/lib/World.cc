
#include "World.hh"
#include "Constants.hh"
#include "VectorUtils.hh"
#include <deque>

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
  handleCollisions();
  removeInvadingEnemies();
  removeEmptyWaves();
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

namespace {
auto rectFromPositionAndDimensions(const Eigen::Vector3f &position,
                                   const Eigen::Vector3f &dimensions) -> SDL_FRect
{
  return SDL_FRect{
    .x = position(0) - dimensions(0) / 2.0f,
    .y = position(1) - dimensions(1) / 2.0f,
    .w = dimensions(0),
    .h = dimensions(1),
  };
}
} // namespace

void World::handleCollisions()
{
  std::deque<std::size_t> bulletsToRemove{};

  for (std::size_t id = 0u; id < m_bullets.size(); ++id)
  {
    auto bulletRect = rectFromPositionAndDimensions(m_bullets[id], bulletDimensions());

    for (auto &wave : m_waves)
    {
      std::deque<std::size_t> enemiesToRemove{};

      for (std::size_t idE = 0u; idE < wave.enemies.size(); ++idE)
      {
        auto enemyRect = rectFromPositionAndDimensions(wave.enemies[idE].pos, enemyDimensions());

        if (SDL_HasRectIntersectionFloat(&bulletRect, &enemyRect))
        {
          bulletsToRemove.push_front(id);
          enemiesToRemove.push_front(idE);
        }
      }

      for (const auto &id : enemiesToRemove)
      {
        wave.enemies.erase(wave.enemies.begin() + id);
      }
    }
  }

  for (const auto &id : bulletsToRemove)
  {
    m_bullets.erase(m_bullets.begin() + id);
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

void World::removeEmptyWaves()
{
  std::erase_if(m_waves, [this](const Wave &wave) { return wave.empty(); });
}

} // namespace invaderz
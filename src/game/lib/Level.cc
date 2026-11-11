
#include "Level.hh"
#include "Constants.hh"
#include "VectorUtils.hh"
#include <SDL3/SDL.h>
#include <deque>

namespace invaderz {
namespace {
constexpr float SPACE_BETWEEN_WAVES = 80.0f;

constexpr auto PLAYER_BULLET_DAMAGAE = 1;

// The speeds are expressed in pixels per second
constexpr auto PLAYER_SPEED = 280;
constexpr auto BULLET_SPEED = 800;
} // namespace

Level::Level(const int levelIndex, const int wavesCount, const Eigen::Vector3f &worldDims)
  : ILevel()
  , runtime::CoreObject("level")
  , m_dimensions(worldDims)
{
  // Offset the y position of the player so that it is fully visible.
  const auto yOffset = playerDimensions()(1) / 2.0f + 4;
  m_player           = Eigen::Vector3f(m_dimensions(0) / 2.0f, yOffset, 0.0f);
  info("player pos " + str(m_player));

  for (int id = 0; id < wavesCount; ++id)
  {
    m_waves.emplace_back(m_dimensions, id * SPACE_BETWEEN_WAVES, levelIndex);
  }
}

auto Level::dims() const -> const Eigen::Vector3f &
{
  return m_dimensions;
}

auto Level::playerPosition() const -> const Eigen::Vector3f &
{
  return m_player;
}

auto Level::remainingWaves() const -> int
{
  return static_cast<int>(m_waves.size());
}

void Level::movePlayer(const Eigen::Vector3f &motion)
{
  m_player += (motion * PLAYER_SPEED);
  if (m_player(0) < 0.0f)
  {
    m_player(0) = 0.0f;
  }
  if (m_player(0) > m_dimensions(0))
  {
    m_player(0) = m_dimensions(0);
  }
}

void Level::fire()
{
  m_bullets.emplace_back(PLAYER_BULLET_DAMAGAE, m_player, false);
}

namespace {
constexpr auto REASONABLE_FRAME_TIME = 100.0f;
}

void Level::update(GameState &state, const float elapsed, Effects &effects)
{
  // The first frame might receive a very long elapsed time
  // which breaks the logic relying on it being more or less
  // consistent and small.
  // This logic helps to ignore outliers where the elapsed
  // time since the last frame is too big.
  if (elapsed > REASONABLE_FRAME_TIME)
  {
    warn("Received elapsed time " + std::to_string(elapsed) + " bigger than threshold "
         + std::to_string(REASONABLE_FRAME_TIME) + ", ignoring update");
    return;
  }

  fireEnemiesGuns(elapsed);
  moveBullets(elapsed);
  removeOutOfBoundsBullets();
  moveEnemies(elapsed);
  handleCollisions(state, effects);
  removeInvadingEnemies();
  removeEmptyWaves();
}

auto Level::bullets() const -> std::vector<Bullet>
{
  return m_bullets;
}

auto Level::enemies() const -> std::vector<Eigen::Vector3f>
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

void Level::fireEnemiesGuns(const float elapsed)
{
  for (auto &wave : m_waves)
  {
    for (std::size_t idE = 0u; idE < wave.enemies.size(); ++idE)
    {
      auto &enemy = wave.enemies[idE];

      enemy.gun.sinceLastFire += elapsed;

      if (enemy.gun.sinceLastFire >= enemy.gun.firingInterval)
      {
        m_bullets.emplace_back(enemy.gun.damage, enemy.pos, true);
        enemy.gun.sinceLastFire = 0;
      }
    }
  }
}

void Level::moveBullets(const float elapsed)
{
  Eigen::Vector3f motion(0, elapsed * BULLET_SPEED, 0.0f);

  for (auto &bullet : m_bullets)
  {
    if (bullet.canDamagePlayer == true)
    {
      bullet.pos -= motion;
    }
    else
    {
      bullet.pos += motion;
    }
  }
}

void Level::removeOutOfBoundsBullets()
{
  std::erase_if(m_bullets, [this](const Bullet &bullet) { return bullet.pos(1) > m_dimensions(1); });
}

void Level::moveEnemies(const float elapsed)
{
  for (auto &wave : m_waves)
  {
    wave.move(elapsed, m_dimensions);
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

void Level::handleCollisions(GameState &state, Effects &effects)
{
  std::deque<std::size_t> bulletsToRemove{};

  // Enemies <-> bullets collisions
  for (std::size_t id = 0u; id < m_bullets.size(); ++id)
  {
    if (m_bullets[id].canDamagePlayer == true)
    {
      continue;
    }

    auto bulletRect    = rectFromPositionAndDimensions(m_bullets[id].pos, bulletDimensions());
    bool bulletRemoved = false;

    for (auto &wave : m_waves)
    {
      std::deque<std::size_t> enemiesToRemove{};

      for (std::size_t idE = 0u; idE < wave.enemies.size(); ++idE)
      {
        auto enemyRect = rectFromPositionAndDimensions(wave.enemies[idE].pos, enemyDimensions());

        if (SDL_HasRectIntersectionFloat(&bulletRect, &enemyRect))
        {
          wave.enemies[idE].health -= m_bullets[id].damage;

          if (!bulletRemoved)
          {
            bulletsToRemove.push_front(id);
            bulletRemoved = true;
          }

          if (wave.enemies[idE].health <= 0)
          {
            state.score += wave.enemies[idE].worth;
            effects.explosions.emplace_back(wave.enemies[idE].pos);

            enemiesToRemove.push_front(idE);
          }
        }
      }

      for (const auto &idToRemove : enemiesToRemove)
      {
        wave.enemies.erase(wave.enemies.begin() + idToRemove);
      }
    }
  }

  for (const auto &id : bulletsToRemove)
  {
    m_bullets.erase(m_bullets.begin() + id);
  }

  auto playerRect = rectFromPositionAndDimensions(m_player, playerDimensions());

  for (auto &wave : m_waves)
  {
    std::deque<std::size_t> enemiesToRemove{};

    for (std::size_t idE = 0u; idE < wave.enemies.size(); ++idE)
    {
      auto enemyRect = rectFromPositionAndDimensions(wave.enemies[idE].pos, enemyDimensions());

      if (SDL_HasRectIntersectionFloat(&playerRect, &enemyRect))
      {
        --state.lives;
        warn("Lost a live!");
        enemiesToRemove.push_front(idE);
      }
    }

    for (const auto &id : enemiesToRemove)
    {
      wave.enemies.erase(wave.enemies.begin() + id);
    }
  }
}

void Level::removeInvadingEnemies()
{
  for (auto &wave : m_waves)
  {
    wave.cleanInvadingEnemies();
  }
}

void Level::removeEmptyWaves()
{
  std::erase_if(m_waves, [this](const Wave &wave) { return wave.empty(); });
}

} // namespace invaderz


#include "BossLevel.hh"
#include "Constants.hh"
#include "VectorUtils.hh"
#include <SDL3/SDL.h>
#include <deque>

namespace invaderz {
namespace {
constexpr auto BOSS_Y_OFFSET = 200.0f;

constexpr auto BOSS_BASE_WORTH  = 1000;
constexpr auto BOSS_BASE_HEALTH = 10;

constexpr auto PLAYER_BULLET_DAMAGAE = 1;

constexpr auto BOSS_BULLET_COUNT = 3;

// The speeds are expressed in pixels per second
constexpr auto PLAYER_SPEED        = 280;
constexpr auto PLAYER_BULLET_SPEED = 800;
constexpr auto BOSS_BULLET_SPEED   = 150;
} // namespace

BossLevel::BossLevel(const int levelIndex, const Eigen::Vector3f &worldDims)
  : ILevel()
  , runtime::CoreObject("boss")
  , m_dimensions(worldDims)
{
  // Offset the y position of the player so that it is fully visible.
  const auto yOffset = playerDimensions()(1) / 2.0f + 4;
  m_player           = Eigen::Vector3f(m_dimensions(0) / 2.0f, yOffset, 0.0f);
  info("player pos " + str(m_player));

  const auto x = worldDims(0) / 2.0f;

  m_boss = Enemy{.xInit  = x,
                 .worth  = BOSS_BASE_WORTH,
                 .health = BOSS_BASE_HEALTH * levelIndex,
                 .pos    = Eigen::Vector3f(x, worldDims(1) - BOSS_Y_OFFSET, 0.0f),
                 .gun    = Gun{
                      .firingInterval = 2.0f,
                      .sinceLastFire  = 0,
                      .damage         = 2,
                 }};
}

auto BossLevel::dims() const -> const Eigen::Vector3f &
{
  return m_dimensions;
}

auto BossLevel::playerPosition() const -> const Eigen::Vector3f &
{
  return m_player;
}

auto BossLevel::remainingWaves() const -> int
{
  return m_boss.has_value() ? 1 : 0;
}

void BossLevel::movePlayer(const Eigen::Vector3f &motion)
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

void BossLevel::fire()
{
  m_bullets.emplace_back(PLAYER_BULLET_DAMAGAE, m_player);
}

namespace {
constexpr auto REASONABLE_FRAME_TIME = 100.0f;
}

void BossLevel::update(GameState &state, const float elapsed, Effects &effects)
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

auto BossLevel::bullets() const -> std::vector<Bullet>
{
  return m_bullets;
}

auto BossLevel::enemies() const -> std::vector<Eigen::Vector3f>
{
  if (m_boss.has_value())
  {
    return std::vector<Eigen::Vector3f>{m_boss->pos};
  }
  return std::vector<Eigen::Vector3f>{};
}

void BossLevel::fireEnemiesGuns(const float elapsed)
{
  m_boss->gun.sinceLastFire += elapsed;

  if (m_boss->gun.sinceLastFire < m_boss->gun.firingInterval)
  {
    return;
  }

  m_boss->gun.sinceLastFire = 0;

  const auto xInit    = m_boss->pos(0) - bossDimensions()(0) / 2.0f;
  const auto y        = m_boss->pos(1) - bossDimensions()(1) / 2.0f;
  const auto interval = bossDimensions()(0) / BOSS_BULLET_COUNT;

  for (auto id = 0; id < BOSS_BULLET_COUNT; ++id)
  {
    const auto x = xInit + id * interval;
    Eigen::Vector3f bulletPos(x, y, 0.0f);
    m_bullets.emplace_back(m_boss->gun.damage, bulletPos, true);
  }
}

void BossLevel::moveBullets(const float elapsed)
{
  Eigen::Vector3f playerMotion(0, elapsed * PLAYER_BULLET_SPEED, 0.0f);
  Eigen::Vector3f bossMotion(0, elapsed * BOSS_BULLET_SPEED, 0.0f);

  for (auto &bullet : m_bullets)
  {
    if (bullet.canDamagePlayer == true)
    {
      bullet.pos -= bossMotion;
    }
    else
    {
      bullet.pos += playerMotion;
    }
  }
}

void BossLevel::removeOutOfBoundsBullets()
{
  std::erase_if(m_bullets, [this](const Bullet &bullet) { return bullet.pos(1) > m_dimensions(1); });
}

namespace {
// Expressed in rotqtion per second.
constexpr auto BOSS_ROTATIONAL_SPEED = 0.33;
constexpr auto BOSS_MOTION_RADIUS    = 50.0f;
} // namespace

void BossLevel::moveEnemies(const float elapsed)
{
  m_totalElapsed += elapsed;

  const auto x = m_boss->xInit
                 + BOSS_MOTION_RADIUS
                     * std::cos(m_totalElapsed * BOSS_ROTATIONAL_SPEED * std::numbers::pi_v<float>);
  const auto y = m_dimensions(1) - BOSS_Y_OFFSET
                 - BOSS_MOTION_RADIUS
                     * std::sin(m_totalElapsed * BOSS_ROTATIONAL_SPEED * std::numbers::pi_v<float>);
  m_boss->pos = Eigen::Vector3f(x, y, 0.0f);
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

void BossLevel::handleCollisions(GameState &state, Effects &effects)
{
  std::deque<std::size_t> bulletsToRemove{};

  bool bossKilled{false};

  // Enemies <-> bullets collisions
  for (std::size_t id = 0u; id < m_bullets.size(); ++id)
  {
    auto bulletRect    = rectFromPositionAndDimensions(m_bullets[id].pos, bulletDimensions());
    bool bulletRemoved = false;

    // TODO: Ideally it should be a more complex bbox instead of just a rectangle to adapt
    // for the shape of the boss.
    auto enemyRect = rectFromPositionAndDimensions(m_boss->pos, 0.25f * bossDimensions());

    if (SDL_HasRectIntersectionFloat(&bulletRect, &enemyRect))
    {
      m_boss->health -= m_bullets[id].damage;

      if (!bulletRemoved)
      {
        bulletsToRemove.push_front(id);
        bulletRemoved = true;
      }

      if (m_boss->health <= 0)
      {
        state.score += m_boss->worth;
        effects.explosions.emplace_back(m_boss->pos);

        bossKilled = true;

        break;
      }
    }
  }

  // Player <-> bullets collisions
  for (std::size_t id = 0u; id < m_bullets.size(); ++id)
  {
    if (m_bullets[id].canDamagePlayer != true)
    {
      continue;
    }

    auto bulletRect    = rectFromPositionAndDimensions(m_bullets[id].pos, bulletDimensions());
    bool bulletRemoved = false;

    auto playerRect = rectFromPositionAndDimensions(m_player, playerDimensions());

    if (SDL_HasRectIntersectionFloat(&bulletRect, &playerRect))
    {
      state.lives -= m_bullets[id].damage;

      if (!bulletRemoved)
      {
        bulletsToRemove.push_front(id);
        bulletRemoved = true;
      }

      // TODO: Maybe have another explosion?
      effects.explosions.emplace_back(m_player);
    }
  }

  if (bossKilled)
  {
    m_boss.reset();
  }

  for (const auto &id : bulletsToRemove)
  {
    m_bullets.erase(m_bullets.begin() + id);
  }
}

void BossLevel::removeInvadingEnemies()
{
  // Voluntarily empty
}

void BossLevel::removeEmptyWaves()
{
  // Voluntarily empty
}

} // namespace invaderz

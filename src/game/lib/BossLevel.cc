
#include "BossLevel.hh"
#include "Constants.hh"
#include "VectorUtils.hh"
#include <SDL3/SDL.h>
#include <deque>

namespace invaderz {
namespace {
constexpr auto BOSS_Y_OFFSET = 200.0f;

// The speeds are expressed in pixels per second
constexpr auto PLAYER_SPEED = 280;
constexpr auto BULLET_SPEED = 800;
} // namespace

BossLevel::BossLevel(const Eigen::Vector3f &worldDims)
  : ILevel()
  , runtime::CoreObject("boss")
  , m_dimensions(worldDims)
{
  // Offset the y position of the player so that it is fully visible.
  const auto yOffset = playerDimensions()(1) / 2.0f + 4;
  m_player           = Eigen::Vector3f(m_dimensions(0) / 2.0f, yOffset, 0.0f);
  info("player pos " + str(m_player));

  const auto x = worldDims(0) / 2.0f;

  m_boss = Enemy{
    .xInit = x,
    .worth = 1000,
    .pos   = Eigen::Vector3f(x, worldDims(1) - BOSS_Y_OFFSET, 0.0f),
  };
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
  m_bullets.emplace_back(m_player);
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

  moveBullets(elapsed);
  removeOutOfBoundsBullets();
  moveEnemies(elapsed);
  handleCollisions(state, effects);
  removeInvadingEnemies();
  removeEmptyWaves();
}

auto BossLevel::bullets() const -> const std::vector<Eigen::Vector3f> &
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

void BossLevel::moveBullets(const float elapsed)
{
  Eigen::Vector3f motion(0, elapsed * BULLET_SPEED, 0.0f);

  for (auto &bullet : m_bullets)
  {
    bullet += motion;
  }
}

void BossLevel::removeOutOfBoundsBullets()
{
  std::erase_if(m_bullets,
                [this](const Eigen::Vector3f &bullet) { return bullet(1) > m_dimensions(1); });
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

  // Enemies <-> bullets collisions
  for (std::size_t id = 0u; id < m_bullets.size(); ++id)
  {
    auto bulletRect    = rectFromPositionAndDimensions(m_bullets[id], bulletDimensions());
    bool bulletRemoved = false;

    bool bossKilled{false};

    // TODO: Ideally it should be a more complex bbox instead of just a rectangle to adapt
    // for the shape of the boss.
    auto enemyRect = rectFromPositionAndDimensions(m_boss->pos, 0.25f * bossDimensions());

    if (SDL_HasRectIntersectionFloat(&bulletRect, &enemyRect))
    {
      if (!bulletRemoved)
      {
        bulletsToRemove.push_front(id);
        bulletRemoved = true;
      }

      state.score += m_boss->worth;
      effects.explosions.emplace_back(m_boss->pos);

      bossKilled = true;
    }

    if (bossKilled)
    {
      m_boss.reset();
    }
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

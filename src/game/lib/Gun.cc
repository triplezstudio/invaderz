
#include "Gun.hh"
#include <random>

namespace invaderz {
namespace {
constexpr auto MIN_FIRING_SPREAD = 0.25f;
constexpr auto MAX_FIRING_SPREAD = 5.0f;
constexpr auto ENEMY_BASE_DAMAGE = 1;

auto generateJitter() -> float
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> distribution(MIN_FIRING_SPREAD, MAX_FIRING_SPREAD);

  return distribution(gen);
}
} // namespace

auto createLevelBasedGun(const int levelMultiplier, const float bulletSpeed) -> Gun
{
  const auto firingInterval = 1.0f / levelMultiplier;

  return Gun{
    .strategy           = FiringStrategy::JITTERED,
    .baseFiringInterval = firingInterval,
    .nextFiringInterval = firingInterval + generateJitter(),
    .sinceLastFire      = 0.0f,
    .damage             = ENEMY_BASE_DAMAGE * levelMultiplier,
    .bulletSpeed        = bulletSpeed,
  };
}

auto Gun::tryFire(const float elapsed, const Eigen::Vector3f position) -> std::vector<Bullet>
{
  sinceLastFire += elapsed;
  if (sinceLastFire < nextFiringInterval)
  {
    return {};
  }

  prepareForNextShot();

  return fire(position);
}

void Gun::prepareForNextShot()
{
  sinceLastFire = 0;

  if (strategy == FiringStrategy::JITTERED)
  {
    auto next          = baseFiringInterval + generateJitter();
    nextFiringInterval = next;
  }
}

namespace {
auto fireSingleShot(const Eigen::Vector3f position, const int damage, const float bulletSpeed)
  -> std::vector<Bullet>
{
  return {
    Bullet{
      .damage          = damage,
      .pos             = position,
      .speed           = bulletSpeed * Eigen::Vector3f(0.0f, -1.0f, 0.0f),
      .canDamagePlayer = true,
    },
  };
}

constexpr auto BAR_BULLET_SPREAD = 380.0f;
constexpr auto BAR_BULLET_COUNT  = 3;

auto fireBarShot(const Eigen::Vector3f position, const int damage, const float bulletSpeed)
  -> std::vector<Bullet>
{
  const auto xInit    = position(0) - BAR_BULLET_SPREAD / 2.0f;
  const auto y        = position(1);
  const auto interval = BAR_BULLET_SPREAD / BAR_BULLET_COUNT;

  std::vector<Bullet> out{};

  for (auto id = 0; id < BAR_BULLET_COUNT; ++id)
  {
    const auto x = xInit + id * interval;

    Bullet bullet{
      .damage          = damage,
      .pos             = Eigen::Vector3f(x, y, 0.0f),
      .speed           = bulletSpeed * Eigen::Vector3f(0.0f, -1.0f, 0.0f),
      .canDamagePlayer = true,
    };

    out.emplace_back(bullet);
  }

  return out;
}

constexpr auto STAR_BULLET_COUNT = 15;

auto fireStarShot(const Eigen::Vector3f position, const int damage, const float bulletSpeed)
  -> std::vector<Bullet>
{
  std::vector<Bullet> out{};

  for (auto id = 0; id < STAR_BULLET_COUNT; ++id)
  {
    Eigen::Vector2f start(1.0f, 0.0f);
    Eigen::Rotation2Df rotation(-std::numbers::pi_v<float> * id / STAR_BULLET_COUNT);

    Eigen::Vector2f direction = rotation * start;

    Bullet bullet{
      .damage          = damage,
      .pos             = position,
      .speed           = bulletSpeed * Eigen::Vector3f(direction(0), direction(1), 0.0f),
      .canDamagePlayer = true,
    };

    out.emplace_back(bullet);
  }

  return out;
}

auto fireSprayShot(const Eigen::Vector3f /*position*/,
                   const int /*damage*/,
                   const float /*bulletSpeed*/) -> std::vector<Bullet>
{
  // TODO: Implement pattern
  return {};
}
} // namespace

auto Gun::fire(const Eigen::Vector3f position) -> std::vector<Bullet>
{
  switch (pattern)
  {
    case FiringPattern::SINGLE:
      return fireSingleShot(position, damage, bulletSpeed);
    case FiringPattern::BAR:
      return fireBarShot(position, damage, bulletSpeed);
    case FiringPattern::STAR:
      return fireStarShot(position, damage, bulletSpeed);
    case FiringPattern::SPRAY:
      return fireSprayShot(position, damage, bulletSpeed);
    default:
      throw std::runtime_error("Unexpected firing pattern "
                               + std::to_string(static_cast<int>(pattern)));
  }
}

} // namespace invaderz

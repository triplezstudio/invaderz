
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

auto createLevelBasedGun(const int levelMultiplier) -> Gun
{
  const auto firingInterval = 1.0f / levelMultiplier;

  return Gun{
    .strategy           = FiringStrategy::JITTERED,
    .baseFiringInterval = firingInterval,
    .nextFiringInterval = firingInterval + generateJitter(),
    .sinceLastFire      = 0.0f,
    .damage             = ENEMY_BASE_DAMAGE * levelMultiplier,
  };
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

} // namespace invaderz

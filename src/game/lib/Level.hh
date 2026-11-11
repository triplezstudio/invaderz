
#pragma once

#include "Bullet.hh"
#include "CoreObject.hh"
#include "ILevel.hh"
#include "Wave.hh"
#include <eigen3/Eigen/Eigen>
#include <vector>

namespace invaderz {

class Level : public ILevel, public runtime::CoreObject
{
  public:
  Level(const int levelIndex, const int wavesCount, const Eigen::Vector3f &worldDims);
  ~Level() override = default;

  auto dims() const -> const Eigen::Vector3f & override;
  auto playerPosition() const -> const Eigen::Vector3f & override;
  auto remainingWaves() const -> int override;

  void movePlayer(const Eigen::Vector3f &motion) override;
  void fire() override;

  void update(GameState &state, const float elapsed, Effects &effects) override;

  auto bullets() const -> std::vector<Eigen::Vector3f> override;
  auto enemies() const -> std::vector<Eigen::Vector3f> override;

  private:
  Eigen::Vector3f m_dimensions{};
  std::vector<Wave> m_waves{};
  std::vector<Bullet> m_bullets{};

  Eigen::Vector3f m_player{};

  void moveBullets(const float elapsed);
  void removeOutOfBoundsBullets();
  void moveEnemies(const float elapsed);
  void handleCollisions(GameState &state, Effects &effects);
  void removeInvadingEnemies();
  void removeEmptyWaves();
};

} // namespace invaderz

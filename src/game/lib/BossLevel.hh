
#pragma once

#include "CoreObject.hh"
#include "Enemy.hh"
#include "ILevel.hh"
#include <eigen3/Eigen/Eigen>
#include <vector>

namespace invaderz {

class BossLevel : public ILevel, public runtime::CoreObject
{
  public:
  BossLevel(const int levelIndex, const Eigen::Vector3f &worldDims);
  ~BossLevel() override = default;

  auto dims() const -> const Eigen::Vector3f & override;
  auto playerPosition() const -> const Eigen::Vector3f & override;
  auto remainingWaves() const -> int override;

  void movePlayer(const Eigen::Vector3f &motion) override;
  void fire() override;

  void update(GameState &state, const float elapsed, Effects &effects) override;

  auto bullets() const -> std::vector<Bullet> override;
  auto enemies() const -> std::vector<Eigen::Vector3f> override;

  private:
  Eigen::Vector3f m_dimensions{};
  std::optional<Enemy> m_boss{};

  std::vector<Bullet> m_bullets{};

  Eigen::Vector3f m_player{};

  float m_totalElapsed{};

  void fireEnemiesGuns(const float elapsed);
  void moveBullets(const float elapsed);
  void removeOutOfBoundsBullets();
  void moveEnemies(const float elapsed);
  void handleCollisions(GameState &state, Effects &effects);
  void removeInvadingEnemies();
  void removeEmptyWaves();
};

} // namespace invaderz

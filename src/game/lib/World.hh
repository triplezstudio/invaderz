
#pragma once

#include "CoreObject.hh"
#include "FrameData.hh"
#include "Level.hh"
#include "Wave.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <vector>

namespace invaderz {

class World : public runtime::CoreObject
{
  public:
  World(Level level);
  ~World() override = default;

  auto dims() const -> const Eigen::Vector3f &;
  auto playerPosition() const -> const Eigen::Vector3f &;

  auto bullets() const -> const std::vector<Eigen::Vector3f> &;
  auto enemies() const -> std::vector<Eigen::Vector3f>;

  void movePlayer(const Eigen::Vector3f &motion);
  void fire();

  /// @brief - Update the world after elapsed seconds have passed.
  /// @param elapsed - the elapsed time since the last call to this
  /// function in seconds.
  void update(const float elapsed);

  private:
  Level m_level;

  Eigen::Vector3f m_player{};
  std::vector<Eigen::Vector3f> m_bullets{};

  float m_elapsedSinceLastEnemyWave{};

  void initialize();

  void moveEnemies(const float elapsed);
  void handleCollisions();
  void removeOutOfBoundsBullets();
  void removeInvadingEnemies();
  void removeEmptyWaves();
};

using WorldPtr = std::unique_ptr<World>;

} // namespace invaderz

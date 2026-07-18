
#pragma once

#include "CoreObject.hh"
#include "FrameData.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <vector>

namespace invaderz {

class World : public runtime::CoreObject
{
  public:
  World(Eigen::Vector3f dims);
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
  Eigen::Vector3f m_dims{};

  std::vector<Eigen::Vector3f> m_bullets{};
  std::vector<Eigen::Vector3f> m_invaders{};

  int m_lastWaveId{};
  struct Enemy
  {
    int waveId{};
    float xInit{};
    Eigen::Vector3f pos{};
  };
  std::vector<Enemy> m_enemies{};

  Eigen::Vector3f m_player{};

  float m_elapsedSinceLastEnemyWave{};

  void initialize();

  void maybeSpawnEnemyWave(const float elapsed);
  void moveEnemies(const float elapsed);
  void removeOutOfBoundsBullets();
  void removeInvadingEnemies();
};

using WorldPtr = std::unique_ptr<World>;

} // namespace invaderz

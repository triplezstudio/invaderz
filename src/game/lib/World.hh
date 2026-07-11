
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
  Eigen::Vector3f m_player{};

  void initialize();
};

using WorldPtr = std::unique_ptr<World>;

} // namespace invaderz

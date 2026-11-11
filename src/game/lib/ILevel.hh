
#pragma once

#include "Bullet.hh"
#include "Effects.hh"
#include "GameState.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>

namespace invaderz {

class ILevel
{
  public:
  ILevel()          = default;
  virtual ~ILevel() = default;

  virtual auto dims() const -> const Eigen::Vector3f &           = 0;
  virtual auto playerPosition() const -> const Eigen::Vector3f & = 0;
  virtual auto remainingWaves() const -> int                     = 0;

  virtual void movePlayer(const Eigen::Vector3f &motion) = 0;
  virtual void fire()                                    = 0;

  virtual void update(GameState &state, const float elapsed, Effects &effects) = 0;

  virtual auto bullets() const -> std::vector<Bullet>          = 0;
  virtual auto enemies() const -> std::vector<Eigen::Vector3f> = 0;
};

using ILevelPtr = std::unique_ptr<ILevel>;

} // namespace invaderz

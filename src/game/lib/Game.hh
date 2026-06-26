
#pragma once

#include "CoreObject.hh"
#include "EventData.hh"
#include "IRenderer.hh"
#include "IAudio.hh"
#include <eigen3/Eigen/Eigen>

namespace invaderz {

class Game : public runtime::CoreObject
{
  public:
  Game(const Eigen::Vector3f &worldDims);
  ~Game() override = default;

  bool update(const EventData &data);
  void processSound(IAudio &audio);
  void render(IRenderer &renderer);

  private:
  Eigen::Vector3f m_worldDims{Eigen::Vector3f::Zero()};
  Eigen::Vector3f m_playerPosition{Eigen::Vector3f::Zero()};
  std::unique_ptr<WaveData> m_mainTheme;

  void initialize();

};

} // namespace invaderz


#pragma once

#include "CoreObject.hh"
#include "FrameData.hh"
#include "IAudioEngine.hh"
#include "IAudioManager.hh"
#include "IRenderer.hh"
#include <eigen3/Eigen/Eigen>

namespace invaderz {

class Game : public runtime::CoreObject
{
  public:
  Game(const Eigen::Vector3f &worldDims);
  ~Game() override = default;

  void loadResources(IAudioManager &manager);

  bool update(const FrameData &data);
  void processSounds(IAudioEngine &engine);
  void render(IRenderer &renderer);

  private:
  Eigen::Vector3f m_worldDims{Eigen::Vector3f::Zero()};
  Eigen::Vector3f m_playerPosition{Eigen::Vector3f::Zero()};
  assets::Asset m_mainTheme{};

  void initialize();
};

} // namespace invaderz

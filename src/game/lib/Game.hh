
#pragma once

#include "CoreObject.hh"
#include "FrameData.hh"
#include "IAudioEngine.hh"
#include "IAudioManager.hh"
#include "IRenderer.hh"
#include "ITextureManager.hh"
#include "PlayerUpdater.hh"
#include "World.hh"
#include <eigen3/Eigen/Eigen>

namespace invaderz {

class Game : public runtime::CoreObject
{
  public:
  Game(Eigen::Vector3f screenDims);
  ~Game() override = default;

  void loadSounds(IAudioManager &manager);
  void loadTextures(ITextureManager &manager);

  bool update(const FrameData &data);
  void processSounds(IAudioEngine &engine);
  void render(IRenderer &renderer);

  private:
  assets::Asset m_mainTheme{};
  assets::Asset m_shipTexture{};

  Eigen::Vector3f m_screenDims{};
  WorldPtr m_world{};
  PlayerUpdaterPtr m_playerUpdater{};

  void initialize(Eigen::Vector3f screenDims);
};

} // namespace invaderz

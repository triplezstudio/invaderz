
#pragma once

#include "CoreObject.hh"
#include "FrameData.hh"
#include "IAudioEngine.hh"
#include "IAudioRegistry.hh"
#include "IFontRegistry.hh"
#include "IRenderer.hh"
#include "ITextureRegistry.hh"
#include "PlayerUpdater.hh"
#include "Screen.hh"
#include "World.hh"
#include <eigen3/Eigen/Eigen>

namespace invaderz {

class Game : public runtime::CoreObject
{
  public:
  Game(Eigen::Vector3f screenDims);
  ~Game() override = default;

  void loadSounds(IAudioRegistry &registry);
  void loadTextures(ITextureRegistry &registry);
  void loadFonts(IFontRegistry &registry);

  bool update(const FrameData &data);
  void processSounds(IAudioEngine &engine);
  void render(IRenderer &renderer);

  private:
  Screen m_screen{Screen::WELCOME};

  SoundId m_mainTheme{};
  TextureId m_title{};
  TextureId m_background{};
  TextureId m_spaceShip{};
  TextureId m_enemyShip{};
  TextureId m_bullet{};
  FontId m_font{};

  Eigen::Vector3f m_screenDims{};
  WorldPtr m_world{};
  PlayerUpdaterPtr m_playerUpdater{};

  void initialize(Eigen::Vector3f screenDims);

  void renderWelcomeScreen(IRenderer &renderer);
  void renderGame(IRenderer &renderer);
  void renderEndGameScreen(IRenderer &renderer);
};

} // namespace invaderz

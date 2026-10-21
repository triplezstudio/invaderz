
#pragma once

#include "CoreObject.hh"
#include "Explosion.hh"
#include "IStage.hh"
#include "PlayerUpdater.hh"
#include "World.hh"

namespace invaderz {

class GameStage : public IStage, public runtime::CoreObject
{
  public:
  GameStage(Eigen::Vector3f screenDims);
  ~GameStage() override = default;

  void loadSounds(IAudioRegistry &registry) override;
  void loadTextures(ITextureRegistry &registry) override;
  void loadFonts(IFontRegistry &registry) override;

  auto update(const FrameData &data) -> StageResult override;
  void processSounds(IAudioEngine &engine) override;
  void render(IRenderer &renderer) override;

  private:
  SoundId m_fireSound{};
  SoundId m_explosionSound{};
  TextureId m_background{};
  TextureId m_spaceShip{};
  TextureId m_enemyShip{};
  TextureId m_bullet{};
  TextureId m_explosion{};
  FontId m_font{};

  Eigen::Vector3f m_screenDims{};

  GameState m_state{};
  bool m_paused{};

  WorldPtr m_world{};
  PlayerUpdaterPtr m_playerUpdater{};

  int m_shotsToFire{};
  int m_explosionsToStart{};
  std::vector<Explosion> m_explosions{};

  void initialize(Eigen::Vector3f screenDims);

  void processActionKeys(const FrameData &data);

  void renderScoreMenu(IRenderer &renderer);
  void renderExplosions(IRenderer &renderer);
};

} // namespace invaderz

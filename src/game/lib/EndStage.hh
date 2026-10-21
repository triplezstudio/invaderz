
#pragma once

#include "IStage.hh"

namespace invaderz {

class EndStage : public IStage
{
  public:
  EndStage(GameState *state, Eigen::Vector3f screenDims);
  ~EndStage() override = default;

  void loadSounds(IAudioRegistry &registry) override;
  void loadTextures(ITextureRegistry &registry) override;
  void loadFonts(IFontRegistry &registry) override;

  auto update(const FrameData &data) -> StageResult override;
  void render(IRenderer &renderer) override;

  private:
  SoundId m_mainTheme{};
  TextureId m_title{};
  FontId m_font{};

  Eigen::Vector3f m_screenDims{};

  auto computeCenteredTextPosition(IRenderer &renderer,
                                   const FontId fontId,
                                   const std::string &text) const -> float;
};

} // namespace invaderz

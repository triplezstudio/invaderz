
#pragma once

#include "FrameData.hh"
#include "GameState.hh"
#include "IAudioEngine.hh"
#include "IAudioRegistry.hh"
#include "IFontRegistry.hh"
#include "IRenderer.hh"
#include "ITextureRegistry.hh"
#include "StageResult.hh"
#include <memory>

namespace invaderz {

class IStage
{
  public:
  IStage(GameState *state);
  virtual ~IStage() = default;

  virtual void loadSounds(IAudioRegistry &registry)     = 0;
  virtual void loadTextures(ITextureRegistry &registry) = 0;
  virtual void loadFonts(IFontRegistry &registry)       = 0;

  virtual auto update(const FrameData &data) -> StageResult = 0;
  virtual void processSounds(IAudioEngine &engine);
  virtual void render(IRenderer &renderer) = 0;

  protected:
  GameState *m_state{};

  auto overrideWithEscapeKey(const FrameData &data, StageResult result) -> StageResult;
};

using IStagePtr = std::unique_ptr<IStage>;

} // namespace invaderz

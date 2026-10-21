
#pragma once

#include "IAudioRegistry.hh"
#include "IFontRegistry.hh"
#include "IStage.hh"
#include "ITextureRegistry.hh"
#include "Screen.hh"
#include <eigen3/Eigen/Eigen>

namespace invaderz {

class StageFactory
{
  public:
  StageFactory(IAudioRegistry &audioRegistry,
               IFontRegistry &fontRegistry,
               ITextureRegistry &textureRegistry,
               Eigen::Vector3f screenDims);
  ~StageFactory() = default;

  auto create(const Screen screen) const -> IStagePtr;

  private:
  IAudioRegistry &m_audioRegistry;
  IFontRegistry &m_fontRegistry;
  ITextureRegistry &m_textureRegistry;

  Eigen::Vector3f m_screenDims{};
};

} // namespace invaderz

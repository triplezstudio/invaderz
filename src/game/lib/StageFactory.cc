
#include "StageFactory.hh"
#include "EndStage.hh"
#include "RegularGameStage.hh"
#include "WelcomeStage.hh"

namespace invaderz {

StageFactory::StageFactory(IAudioRegistry &audioRegistry,
                           IFontRegistry &fontRegistry,
                           ITextureRegistry &textureRegistry,
                           GameState *state,
                           Eigen::Vector3f screenDims)
  : m_audioRegistry(audioRegistry)
  , m_fontRegistry(fontRegistry)
  , m_textureRegistry(textureRegistry)
  , m_state(state)
  , m_screenDims(std::move(screenDims))
{}

auto StageFactory::create(const Screen screen) const -> IStagePtr
{
  IStagePtr stage{};

  switch (screen)
  {
    case Screen::WELCOME:
      stage = std::make_unique<WelcomeStage>(m_state, m_screenDims);
      break;
    case Screen::REGULAR_LEVEL:
      stage = std::make_unique<RegularGameStage>(m_state, m_screenDims);
      break;
    case Screen::BOSS_LEVEL:
      throw std::invalid_argument("Boss level is not implemented");
    case Screen::END_GAME:
      stage = std::make_unique<EndStage>(m_state, m_screenDims);
      break;
    case Screen::EXIT:
      return nullptr;
    default:
      throw std::invalid_argument("Unsupported screen " + str(screen));
  }

  stage->loadSounds(m_audioRegistry);
  stage->loadTextures(m_textureRegistry);
  stage->loadFonts(m_fontRegistry);

  return stage;
}

} // namespace invaderz

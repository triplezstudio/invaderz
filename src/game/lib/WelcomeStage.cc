
#include "WelcomeStage.hh"
#include <format>

namespace invaderz {

WelcomeStage::WelcomeStage(Eigen::Vector3f screenDims)
  : IStage()
  , m_screenDims(screenDims)
{}

void WelcomeStage::loadSounds(IAudioRegistry &registry)
{
  auto themeFilePath = std::format("{}/cyberpunky_theme.wav", std::getenv("ASSET_FOLDER"));
  m_mainTheme        = registry.registerSound(themeFilePath);
}

void WelcomeStage::loadTextures(ITextureRegistry &registry)
{
  auto titleFilePath = std::format("{}/title_screen.png", std::getenv("ASSET_FOLDER"));
  m_title            = registry.registerTexture(titleFilePath);
}

void WelcomeStage::loadFonts(IFontRegistry &registry)
{
  auto filePath = std::format("{}/ArcadeClassic.ttf", std::getenv("ASSET_FOLDER"));
  m_font        = registry.registerFont(filePath, 16);
}

namespace {
const Eigen::Vector3f SCORE_MENU_SIZE(0.0f, 64.0f, 0.0);
}

auto WelcomeStage::update(const FrameData &data) -> StageResult
{
  if (data.state.anyKeyReleased())
  {
    return StageResult{
      .finished = true,
      .next     = Screen::GAME,
    };
  }

  return StageResult{.finished = false};
}

void WelcomeStage::processSounds(IAudioEngine &engine)
{
  static bool initial = true;
  if (initial)
  {
    initial = false;
    engine.playOnce(m_mainTheme, 0.0125f);
  }
}

namespace {
constexpr auto TITLE_TEXT = "Press any key to continue";
} // namespace

void WelcomeStage::render(IRenderer &renderer)
{
  Eigen::Vector3f titleDimensions(m_screenDims(0), m_screenDims(0), 0.0f);
  Eigen::Vector3f position(0.0f, 0.0f, 0.0f);
  renderer.renderTexture(m_title, position, titleDimensions);

  const auto x = computeCenteredTextPosition(renderer, m_font, TITLE_TEXT);
  renderer.renderText(m_font, TITLE_TEXT, Eigen::Vector3f(x, 400.0f, 0.0f));
}

auto WelcomeStage::computeCenteredTextPosition(IRenderer &renderer,
                                               const FontId fontId,
                                               const std::string &text) const -> float
{
  auto &font          = renderer.getFontRegistry().getFont(fontId);
  auto textDimensions = font.getTextSize(text);
  return (m_screenDims(0) - textDimensions(0)) / 2.0f;
}

} // namespace invaderz

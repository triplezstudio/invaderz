
#include "EndStage.hh"
#include <format>

namespace invaderz {

EndStage::EndStage(GameState *state, Eigen::Vector3f screenDims)
  : IStage(state)
  , m_screenDims(screenDims)
{}

void EndStage::loadSounds(IAudioRegistry &registry)
{
  auto themeFilePath = std::format("{}/cyberpunky_theme.wav", std::getenv("ASSET_FOLDER"));
  m_mainTheme        = registry.registerSound(themeFilePath);
}

void EndStage::loadTextures(ITextureRegistry &registry)
{
  auto titleFilePath = std::format("{}/title_screen.png", std::getenv("ASSET_FOLDER"));
  m_title            = registry.registerTexture(titleFilePath);
}

void EndStage::loadFonts(IFontRegistry &registry)
{
  auto filePath = std::format("{}/ArcadeClassic.ttf", std::getenv("ASSET_FOLDER"));
  m_font        = registry.registerFont(filePath, 16);
}

namespace {
const Eigen::Vector3f SCORE_MENU_SIZE(0.0f, 64.0f, 0.0);
}

auto EndStage::update(const FrameData &data) -> StageResult
{
  StageResult result{};

  if (data.state.released(keyboard::RETURN))
  {
    result.finished = true;
    result.next     = Screen::REGULAR_LEVEL;
  }

  return overrideWithEscapeKey(data, result);
}

namespace {
constexpr auto VICTORY_TEXT = "Well done, you won!";
constexpr auto DEFEAT_TEXT  = "Too bad, you lost!";
constexpr auto RESTART_TEXT = "Press Return key continue";
} // namespace

void EndStage::render(IRenderer &renderer)
{
  Eigen::Vector3f titleDimensions(m_screenDims(0), m_screenDims(0), 0.0f);
  Eigen::Vector3f position(0.0f, 0.0f, 0.0f);
  renderer.renderTexture(m_title, position, titleDimensions);

  auto text = VICTORY_TEXT;
  if (m_state->lives <= 0)
  {
    text = DEFEAT_TEXT;
  }

  auto x = computeCenteredTextPosition(renderer, m_font, text);
  renderer.renderText(m_font, text, Eigen::Vector3f(x, 400.0f, 0.0f));

  auto score = std::format("Your score: {}", m_state->score);
  x          = computeCenteredTextPosition(renderer, m_font, score);
  renderer.renderText(m_font, score, Eigen::Vector3f(x, 450.0f, 0.0f));

  x = computeCenteredTextPosition(renderer, m_font, RESTART_TEXT);
  renderer.renderText(m_font, RESTART_TEXT, Eigen::Vector3f(x, 500.0f, 0.0f));
}

auto EndStage::computeCenteredTextPosition(IRenderer &renderer,
                                           const FontId fontId,
                                           const std::string &text) const -> float
{
  auto &font          = renderer.getFontRegistry().getFont(fontId);
  auto textDimensions = font.getTextSize(text);
  return (m_screenDims(0) - textDimensions(0)) / 2.0f;
}

} // namespace invaderz

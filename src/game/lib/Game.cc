
#include "Game.hh"
#include "Constants.hh"
#include "CoordinateConverter.hh"
#include <cstdlib>
#include <format>

namespace invaderz {

Game::Game(Eigen::Vector3f screenDims)
  : runtime::CoreObject("game")
  , m_screenDims(screenDims)
{
  initialize(std::move(screenDims));
}

void Game::loadSounds(IAudioRegistry &registry)
{
  auto themeFilePath = std::format("{}/cyberpunky_theme.wav", std::getenv("ASSET_FOLDER"));
  m_mainTheme        = registry.registerSound(themeFilePath);
}

void Game::loadTextures(ITextureRegistry &registry)
{
  auto titleFilePath = std::format("{}/title_screen.png", std::getenv("ASSET_FOLDER"));
  m_title            = registry.registerTexture(titleFilePath);

  auto backgroundFilePath = std::format("{}/star_background.png", std::getenv("ASSET_FOLDER"));
  m_background            = registry.registerTexture(backgroundFilePath);

  auto spaceshipFilePath = std::format("{}/player_ship.png", std::getenv("ASSET_FOLDER"));
  m_spaceShip            = registry.registerTexture(spaceshipFilePath);

  auto enemyShipFilePath = std::format("{}/enemy_ship.png", std::getenv("ASSET_FOLDER"));
  m_enemyShip            = registry.registerTexture(enemyShipFilePath);

  auto bulletFilePath = std::format("{}/bullet.png", std::getenv("ASSET_FOLDER"));
  m_bullet            = registry.registerTexture(bulletFilePath);

  auto explosionFilePath = std::format("{}/explosion.png", std::getenv("ASSET_FOLDER"));
  m_explosion            = registry.registerTexture(explosionFilePath);
}

void Game::loadFonts(IFontRegistry &registry)
{
  auto filePath = std::format("{}/ArcadeClassic.ttf", std::getenv("ASSET_FOLDER"));
  m_font        = registry.registerFont(filePath, 30);
}

namespace {
const Eigen::Vector3f SCORE_MENU_SIZE(0.0f, 64.0f, 0.0);
}

bool Game::update(const FrameData &data)
{
  if (m_screen == Screen::WELCOME)
  {
    if (data.state.anyKeyReleased())
    {
      info("Transition from welcome screen to game screen");
      m_screen = Screen::GAME;
    }
  }

  if (m_screen == Screen::GAME)
  {
    m_playerUpdater->update(data);

    Effects effects{};
    m_world->update(data.elapsed, effects);

    for (auto &explosion : m_explosions)
    {
      explosion.update(data.elapsed);
    }
    std::erase_if(m_explosions, [](const Explosion &e) { return e.finished(); });
    for (const auto &explosion : effects.explosions)
    {
      Eigen::Vector3f screenOffset(0.0f, SCORE_MENU_SIZE(1), 0.0f);
      Eigen::Vector3f displayDims = m_screenDims - SCORE_MENU_SIZE;
      CoordinateConverter converter{m_world->dims(), screenOffset, displayDims};

      m_explosions.emplace_back(converter.toScreenPos(explosion, explosionDimensions()));
    }

    if (m_world->lives() <= 0 || m_world->remainingWaves() <= 0)
    {
      info("Transition from game screen to end game screen");
      m_screen = Screen::END_GAME;
    }
  }

  if (m_screen == Screen::END_GAME)
  {
    if (data.state.released(keyboard::RETURN))
    {
      info("Transition from end game screen to game screen");
      initialize(m_screenDims);
      m_screen = Screen::GAME;
    }
  }

  processActionKeys(data);

  const auto quit = data.quit || data.state.held(keyboard::ESCAPE);
  return !quit;
}

void Game::processSounds(IAudioEngine &engine)
{
  static bool initial = true;
  if (initial)
  {
    initial = false;
    engine.playOnce(m_mainTheme, 0.0125f);
  }
}

void Game::render(IRenderer &renderer)
{
  switch (m_screen)
  {
    case Screen::WELCOME:
      renderWelcomeScreen(renderer);
      break;
    case Screen::GAME:
    case Screen::PAUSE:
      renderGame(renderer);
      break;
    case Screen::END_GAME:
      renderEndGameScreen(renderer);
      break;
    default:
      error("Unsupported screen " + std::to_string(static_cast<int>(m_screen)));
  }
}

namespace {
constexpr auto PLAYER_LIVES = 15;
constexpr auto WAVES_COUNT  = 5;
} // namespace

void Game::initialize(Eigen::Vector3f screenDims)
{
  Eigen::Vector3f worldDims = screenDims - SCORE_MENU_SIZE;

  Level level(PLAYER_LIVES, WAVES_COUNT, std::move(worldDims));
  m_world         = std::make_unique<World>(std::move(level));
  m_playerUpdater = std::make_unique<PlayerUpdater>(*m_world);
  m_explosions.clear();
}

void Game::processActionKeys(const FrameData &data)
{
  if (data.state.released(keyboard::P))
  {
    if (m_screen == Screen::PAUSE)
    {
      info("Transition from pause screen to game screen");
      m_screen = Screen::GAME;
    }
    else if (m_screen == Screen::GAME)
    {
      info("Transition from game screen to pause screen");
      m_screen = Screen::PAUSE;
    }
  }
}

namespace {
constexpr auto TITLE_TEXT = "Press any key to continue";
} // namespace

void Game::renderWelcomeScreen(IRenderer &renderer)
{
  Eigen::Vector3f titleDimensions(m_screenDims(0), m_screenDims(0), 0.0f);
  Eigen::Vector3f position(0.0f, 0.0f, 0.0f);
  renderer.renderTexture(m_title, position, titleDimensions);

  const auto x = computeCenteredTextPosition(renderer, m_font, TITLE_TEXT);
  renderer.renderText(m_font, TITLE_TEXT, Eigen::Vector3f(x, 400.0f, 0.0f));
}

namespace {
constexpr auto PAUSE_TEXT = "PAUSE";
}

void Game::renderGame(IRenderer &renderer)
{
  renderer.renderTexture(m_background, Eigen::Vector3f::Zero(), m_screenDims);

  Eigen::Vector3f screenOffset(0.0f, SCORE_MENU_SIZE(1), 0.0f);
  Eigen::Vector3f displayDims = m_screenDims - SCORE_MENU_SIZE;
  CoordinateConverter converter{m_world->dims(), screenOffset, displayDims};

  renderer.renderTexture(m_spaceShip,
                         converter.toScreenPos(m_world->playerPosition(), playerDimensions()),
                         playerDimensions());

  renderScoreMenu(renderer);

  renderExplosions(renderer);

  for (const auto &enemy : m_world->enemies())
  {
    renderer.renderTexture(m_enemyShip,
                           converter.toScreenPos(enemy, enemyDimensions()),
                           enemyDimensions());
  }

  for (const auto &bullet : m_world->bullets())
  {
    renderer.renderTexture(m_bullet,
                           converter.toScreenPos(bullet, bulletDimensions()),
                           bulletDimensions(),
                           -90.0f);
  }

  if (m_screen == Screen::PAUSE)
  {
    const auto &font          = renderer.getFontRegistry().getFont(m_font);
    const auto textDimensions = font.getTextSize(PAUSE_TEXT);
    Eigen::Vector3f position((m_screenDims(0) - textDimensions(0)) / 2.0f, 400.0f, 0.0f);
    renderer.renderText(m_font, PAUSE_TEXT, position);
  }
}

namespace {
constexpr auto VICTORY_TEXT = "Well done, you won!";
constexpr auto DEFEAT_TEXT  = "Too bad, you lost!";
constexpr auto RESTART_TEXT = "Press Return key continue";
} // namespace

void Game::renderEndGameScreen(IRenderer &renderer)
{
  Eigen::Vector3f titleDimensions(m_screenDims(0), m_screenDims(0), 0.0f);
  Eigen::Vector3f position(0.0f, 0.0f, 0.0f);
  renderer.renderTexture(m_title, position, titleDimensions);

  auto text = VICTORY_TEXT;
  if (m_world->lives() <= 0)
  {
    text = DEFEAT_TEXT;
  }

  auto x = computeCenteredTextPosition(renderer, m_font, text);
  renderer.renderText(m_font, text, Eigen::Vector3f(x, 400.0f, 0.0f));

  auto score = std::format("Your score: {}", m_world->score());
  x          = computeCenteredTextPosition(renderer, m_font, score);
  renderer.renderText(m_font, score, Eigen::Vector3f(x, 450.0f, 0.0f));

  x = computeCenteredTextPosition(renderer, m_font, RESTART_TEXT);
  renderer.renderText(m_font, RESTART_TEXT, Eigen::Vector3f(x, 500.0f, 0.0f));
}

auto Game::computeCenteredTextPosition(IRenderer &renderer,
                                       const FontId fontId,
                                       const std::string &text) const -> float
{
  auto &font          = renderer.getFontRegistry().getFont(fontId);
  auto textDimensions = font.getTextSize(text);
  return (m_screenDims(0) - textDimensions(0)) / 2.0f;
}

void Game::renderScoreMenu(IRenderer &renderer)
{
  Eigen::Vector3f position(10.0f, 10.0f, 0.0f);
  auto text = std::format("Score {}", m_world->score());
  renderer.renderText(m_font, text, position);

  position = Eigen::Vector3f(m_screenDims(0) / 2.0f, 10.0f, 0.0f);
  text     = std::format("Lives {}", m_world->lives());
  renderer.renderText(m_font, text, position);
}

void Game::renderExplosions(IRenderer &renderer)
{
  for (auto &explosion : m_explosions)
  {
    explosion.render(renderer, m_explosion);
  }
}

} // namespace invaderz

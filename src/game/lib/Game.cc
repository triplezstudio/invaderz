
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

  auto titleLabelFilePath = std::format("{}/title_label.png", std::getenv("ASSET_FOLDER"));
  m_titleLabel            = registry.registerTexture(titleLabelFilePath);

  auto backgroundFilePath = std::format("{}/star_background.png", std::getenv("ASSET_FOLDER"));
  m_background            = registry.registerTexture(backgroundFilePath);

  auto spaceshipFilePath = std::format("{}/player_ship.png", std::getenv("ASSET_FOLDER"));
  m_spaceShip            = registry.registerTexture(spaceshipFilePath);

  auto enemyShipFilePath = std::format("{}/enemy_ship.png", std::getenv("ASSET_FOLDER"));
  m_enemyShip            = registry.registerTexture(enemyShipFilePath);

  auto bulletFilePath = std::format("{}/bullet.png", std::getenv("ASSET_FOLDER"));
  m_bullet            = registry.registerTexture(bulletFilePath);

  auto victoryFilePath = std::format("{}/victory_label.png", std::getenv("ASSET_FOLDER"));
  m_victoryLabel       = registry.registerTexture(victoryFilePath);

  auto lossFilePath = std::format("{}/loss_label.png", std::getenv("ASSET_FOLDER"));
  m_lossLabel       = registry.registerTexture(lossFilePath);
}

void Game::loadFonts(IFontRegistry &registry)
{
  auto filePath = std::format("{}/ArcadeClassic.ttf", std::getenv("ASSET_FOLDER"));
  m_font        = registry.registerFont(filePath, 30);
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
    m_world->update(data.elapsed);

    if (m_world->lives() <= 0 || m_world->remainingWaves() <= 0)
    {
      info("Transition from game screen to end game screen");
      m_screen = Screen::END_GAME;
    }
  }

  if (m_screen == Screen::END_GAME)
  {
    if (data.state.anyKeyReleased())
    {
      info("Transition from end game screen to game screen");
      initialize(m_screenDims);
      m_screen = Screen::GAME;
    }
  }

  const auto quit = data.quit || data.state.held(keyboard::ESCAPE);
  return !quit;
}

void Game::processSounds(IAudioEngine &engine)
{
  static bool initial = true;
  if (initial)
  {
    initial = false;
    engine.playOnce(m_mainTheme, 0.125f);
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
  Eigen::Vector3f worldDims = screenDims - playerDimensions();

  Level level(PLAYER_LIVES, WAVES_COUNT, std::move(worldDims));
  m_world         = std::make_unique<World>(std::move(level));
  m_playerUpdater = std::make_unique<PlayerUpdater>(*m_world);
}

void Game::renderWelcomeScreen(IRenderer &renderer)
{
  Eigen::Vector3f titleDimensions(m_screenDims(0), m_screenDims(0), 0.0f);
  Eigen::Vector3f position(0.0f, 0.0f, 0.0f);
  renderer.renderTexture(m_title, position, titleDimensions);

  // The image has a size of 64 pixels so we can just hard code it.
  titleDimensions = Eigen::Vector3f(300.0f, 64.0f, 0.0f);
  position        = Eigen::Vector3f((m_screenDims(0) - 300.0f) / 2.0f, 400.0f, 0.0f);
  // renderer.renderTexture(m_titleLabel, position, titleDimensions);
  renderer.renderText(m_font, "Press any key to continue", position);
}

void Game::renderGame(IRenderer &renderer)
{
  renderer.renderTexture(m_background, Eigen::Vector3f::Zero(), m_screenDims);

  CoordinateConverter converter{m_world->dims(), m_screenDims};

  renderer.renderTexture(m_spaceShip,
                         converter.toScreenPos(m_world->playerPosition(), playerDimensions()),
                         playerDimensions());

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
}

void Game::renderEndGameScreen(IRenderer &renderer)
{
  Eigen::Vector3f titleDimensions(m_screenDims(0), m_screenDims(0), 0.0f);
  Eigen::Vector3f position(0.0f, 0.0f, 0.0f);
  renderer.renderTexture(m_title, position, titleDimensions);

  titleDimensions = Eigen::Vector3f(300.0f, 64.0f, 0.0f);
  position        = Eigen::Vector3f((m_screenDims(0) - 300.0f) / 2.0f, 400.0f, 0.0f);

  auto texture = m_victoryLabel;
  if (m_world->lives() <= 0)
  {
    texture = m_lossLabel;
  }

  renderer.renderTexture(texture, position, titleDimensions);

  titleDimensions = Eigen::Vector3f(300.0f, 64.0f, 0.0f);
  position        = Eigen::Vector3f((m_screenDims(0) - 300.0f) / 2.0f, 500.0f, 0.0f);
  renderer.renderTexture(m_titleLabel, position, titleDimensions);
}

} // namespace invaderz

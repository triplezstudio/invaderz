
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
  auto spaceshipFilePath = std::format("{}/player_ship.png", std::getenv("ASSET_FOLDER"));
  m_spaceShip            = registry.registerTexture(spaceshipFilePath);

  auto enemyShipFilePath = std::format("{}/enemy_ship.png", std::getenv("ASSET_FOLDER"));
  m_enemyShip            = registry.registerTexture(enemyShipFilePath);

  auto bulletFilePath = std::format("{}/bullet.png", std::getenv("ASSET_FOLDER"));
  m_bullet            = registry.registerTexture(bulletFilePath);
}

bool Game::update(const FrameData &data)
{
  m_playerUpdater->update(data);
  m_world->update(data.elapsed);

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

} // namespace invaderz

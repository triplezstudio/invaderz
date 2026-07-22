
#include "Game.hh"
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

void Game::loadResources(IAudioManager &manager)
{
  auto themeFilePath = std::format("{}/cyberpunky_theme.wav", std::getenv("ASSET_FOLDER"));
  m_mainTheme        = manager.registerSound(themeFilePath);
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
    engine.playOnce(m_mainTheme.id, 0.125f);
  }
}

namespace {
// The dimensions are expressed in pixels.
const Eigen::Vector3f PLAYER_DIMS(32.0f, 32.0f, 0.0f);
const Eigen::Vector3f ENEMY_DIMS(32.0f, 32.0f, 0.0f);
const Eigen::Vector3f BULLET_DIMS(4.0f, 4.0f, 0.0f);
} // namespace

void Game::render(IRenderer &renderer)
{
  CoordinateConverter converter{m_world->dims(), m_screenDims};

  renderer.renderRectangle(converter.toScreenPos(m_world->playerPosition(), PLAYER_DIMS),
                           PLAYER_DIMS,
                           Color::ORANGE);

  for (const auto &enemy : m_world->enemies())
  {
    renderer.renderRectangle(converter.toScreenPos(enemy, ENEMY_DIMS), ENEMY_DIMS, Color::BURGUNDY);
  }

  for (const auto &bullet : m_world->bullets())
  {
    renderer.renderRectangle(converter.toScreenPos(bullet, BULLET_DIMS),
                             BULLET_DIMS,
                             Color::TURQUOISE);
  }
}

void Game::initialize(Eigen::Vector3f screenDims)
{
  Eigen::Vector3f worldDims = screenDims - PLAYER_DIMS;
  m_world                   = std::make_unique<World>(std::move(worldDims));
  m_playerUpdater           = std::make_unique<PlayerUpdater>(*m_world);
}

} // namespace invaderz

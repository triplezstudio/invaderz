
#include "Game.hh"
#include <cstdlib>
#include <format>

namespace invaderz {

Game::Game(const Eigen::Vector3f &worldDims)
  : runtime::CoreObject("game")
  , m_worldDims(worldDims)
{
  initialize();
}

void Game::loadResources(IAudioManager &manager)
{
  auto assetFolder = std::format("{}/cyberpunky_theme.wav", std::getenv("ASSET_FOLDER"));
  m_mainTheme      = manager.registerSound(assetFolder);
}

namespace {
// Player dimensions in pixels
const Eigen::Vector3f PLAYER_DIMS(32.0f, 32.0f, 0.0f);
// Player speed in pixels per second
constexpr auto PLAYER_SPEED = 280;

auto collectMotion(const FrameData &data) -> Eigen::Vector3f
{
  Eigen::Vector3f motion = Eigen::Vector3f::Zero();

  if (data.state.held(keyboard::LEFT))
  {
    motion(0) -= PLAYER_SPEED;
  }
  if (data.state.held(keyboard::RIGHT))
  {
    motion(0) += PLAYER_SPEED;
  }

  motion.normalize();
  motion *= (PLAYER_SPEED * data.elapsed);

  return motion;
}
} // namespace

bool Game::update(const FrameData &data)
{
  auto motion = collectMotion(data);
  m_playerPosition += motion;
  if (m_playerPosition(0) < 0.0f)
  {
    m_playerPosition(0) = 0.0f;
  }
  if (m_playerPosition(0) + PLAYER_DIMS(0) > m_worldDims(0))
  {
    m_playerPosition(0) = m_worldDims(0) - PLAYER_DIMS(0);
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
    info("playing once");
    engine.playOnce(m_mainTheme.id, 1.5f);
  }
}

void Game::render(IRenderer &renderer)
{
  renderer.renderRectangle(m_playerPosition, PLAYER_DIMS);
}

void Game::initialize()
{
  m_playerPosition = Eigen::Vector3f(0.0f, m_worldDims(1) - PLAYER_DIMS(1), 0.0f);
  info("player pos " + std::to_string(m_playerPosition(0)) + "x"
       + std::to_string(m_playerPosition(1)) + "x" + std::to_string(m_playerPosition(2)));
}

} // namespace invaderz

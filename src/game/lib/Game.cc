
#include "Game.hh"
#include "IAudio.hh"
#include <cstdlib>

namespace invaderz {

Game::Game(const Eigen::Vector3f &worldDims)
  : runtime::CoreObject("game")
  , m_worldDims(worldDims)
{
  initialize();
}

namespace {
// Player dimensions in pixels
const Eigen::Vector3f PLAYER_DIMS(32.0f, 32.0f, 0.0f);
// Player speed in pixels per frame
constexpr auto PLAYER_SPEED = 2;

auto collectMotion(const EventData &data) -> Eigen::Vector3f
{
  Eigen::Vector3f motion = Eigen::Vector3f::Zero();

  for (const auto &event : data.events)
  {
    if (event.type == SDL_EVENT_KEY_DOWN)
    {
      switch (event.key.key)
      {
        case SDLK_LEFT:
          motion(0) -= PLAYER_SPEED;
          break;
        case SDLK_RIGHT:
          motion(0) += PLAYER_SPEED;
          break;
        default:
          break;
      }
    }
  }

  motion.normalize();
  motion *= PLAYER_SPEED;

  return motion;
}
} // namespace

bool Game::update(const EventData &data)
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

  return !data.quitRequested();
}

void Game::render(IRenderer &renderer)
{
  renderer.renderRectangle(m_playerPosition, PLAYER_DIMS);
}

namespace {}

void Game::initialize()
{
  m_playerPosition = Eigen::Vector3f(0.0f, m_worldDims(1) - PLAYER_DIMS(1), 0.0f);
  info("player pos " + std::to_string(m_playerPosition(0)) + "x"
       + std::to_string(m_playerPosition(1)) + "x" + std::to_string(m_playerPosition(2)));
}
void Game::processSound(IAudio &audio)
{
  static bool initial = true;
  if (initial)
  {
    initial = false;
    auto assetFolder = std::string(std::getenv("ASSET_FOLDER"));
    mainTheme = audio.loadWavFile(assetFolder + "/cyberpunky_theme.wav");
    audio.playSound(mainTheme.get(), 1.5);
  }



}

} // namespace invaderz


#include "Game.hh"

namespace invaderz {

Game::Game()
  : runtime::CoreObject("game")
{}

namespace {
// Player speed in pixels per frame
constexpr auto PLAYER_SPEED = 2;

bool quitRequested(const EventData &data)
{
  for (const auto &event : data.events)
  {
    if (event.type == SDL_EVENT_QUIT)
    {
      return true;
    }
    if (event.type == SDL_EVENT_KEY_UP && event.key.key == SDLK_ESCAPE)
    {
      return true;
    }
  }

  return false;
}

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
        case SDLK_UP:
          motion(1) -= PLAYER_SPEED;
          break;
        case SDLK_DOWN:
          motion(1) += PLAYER_SPEED;
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

  return !quitRequested(data);
}

namespace {
const Eigen::Vector3f PLAYER_DIMS(32.0f, 32.0f, 0.0f);
}

void Game::render(IRenderer &renderer)
{
  renderer.renderRectangle(m_playerPosition, PLAYER_DIMS);
}

} // namespace invaderz

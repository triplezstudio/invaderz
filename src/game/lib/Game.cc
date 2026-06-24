
#include "Game.hh"

namespace invaderz {

Game::Game()
  : runtime::CoreObject("game")
{}

namespace {
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
} // namespace

bool Game::update(const EventData &data)
{
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

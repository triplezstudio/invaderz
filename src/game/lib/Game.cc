
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
  }

  return false;
}
} // namespace

bool Game::update(const EventData &data)
{
  return !quitRequested(data);
}

void Game::render(IRenderer & /*renderer*/) {}

} // namespace invaderz

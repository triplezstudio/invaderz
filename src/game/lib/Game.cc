
#include "Game.hh"

namespace invaderz {

Game::Game(StageFactory &&factory)
  : runtime::CoreObject("game")
  , m_factory(factory)
{
  m_stage = m_factory.create(Screen::WELCOME);
}

bool Game::update(const FrameData &data)
{
  auto result = m_stage->update(data);
  if (result.next.has_value())
  {
    m_stage = m_factory.create(*result.next);
  }

  return !result.next.has_value() || *result.next != Screen::EXIT;
}

void Game::processSounds(IAudioEngine &engine)
{
  if (m_stage != nullptr)
  {
    m_stage->processSounds(engine);
  }
}

void Game::render(IRenderer &renderer)
{
  if (m_stage != nullptr)
  {
    m_stage->render(renderer);
  }
}

} // namespace invaderz

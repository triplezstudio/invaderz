
#include "IStage.hh"

namespace invaderz {

IStage::IStage(GameState *state)
  : m_state(state)
{}

void IStage::processSounds(IAudioEngine & /*engine*/) {}

auto IStage::overrideWithEscapeKey(const FrameData &data, StageResult result) -> StageResult
{
  if (data.quit || data.state.held(keyboard::ESCAPE))
  {
    return StageResult{.finished = true, .next = Screen::EXIT};
  }

  return result;
}

} // namespace invaderz

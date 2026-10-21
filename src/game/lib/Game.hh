
#pragma once

#include "CoreObject.hh"
#include "IStage.hh"
#include "StageFactory.hh"

namespace invaderz {

class Game : public runtime::CoreObject
{
  public:
  Game(StageFactory &&factory);
  ~Game() override = default;

  bool update(const FrameData &data);
  void processSounds(IAudioEngine &engine);
  void render(IRenderer &renderer);

  private:
  StageFactory m_factory;
  IStagePtr m_stage{};
};

} // namespace invaderz

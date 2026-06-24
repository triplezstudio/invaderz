
#pragma once

#include "CoreObject.hh"
#include "EventData.hh"
#include "IRenderer.hh"

namespace invaderz {

class Game : public runtime::CoreObject
{
  public:
  Game();
  ~Game() override = default;

  bool update(const EventData &data);
  void render(IRenderer &renderer);
};

} // namespace invaderz

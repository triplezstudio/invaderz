
#pragma once

#include "CoreObject.hh"

namespace invaderz {

class App : public runtime::CoreObject
{
  public:
  App();
  ~App() override = default;

  void render();
};

} // namespace invaderz

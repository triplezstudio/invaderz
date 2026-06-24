
#include "App.hh"

namespace invaderz {

App::App()
  : runtime::CoreObject("app")
{}

void App::render()
{
  info("Render");
}

} // namespace invaderz

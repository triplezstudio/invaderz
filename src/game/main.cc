
#include "App.hh"
#include "Game.hh"
#include "Locator.hh"
#include "SdlAssetManager.hh"
#include "StdLogger.hh"
#include <iostream>

int main(int /*argc*/, char * /*argv*/[])
{
  invaderz::log::StdLogger raw;
  raw.setLevel(invaderz::log::Severity::DEBUG);
  invaderz::log::Locator::provide(&raw);

  constexpr auto width  = 480;
  constexpr auto height = 880;

  invaderz::App app(width, height);
  invaderz::Game game(Eigen::Vector3f(1.0f * width, 1.0f * height, 0.0f));

  invaderz::SdlAssetManager manager{};
  game.loadResources(manager);

  bool running = true;
  while (running)
  {
    auto events = app.pollEvents();
    running     = game.update(events);
    app.clear();
    game.processSounds(manager);
    game.render(app);
    manager.update();
    app.render();
  }

  return EXIT_SUCCESS;
}
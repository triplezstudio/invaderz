
#include "App.hh"
#include "Game.hh"
#include "Locator.hh"
#include "StdLogger.hh"
#include <iostream>

int main(int /*argc*/, char * /*argv*/[])
{
  invaderz::log::StdLogger raw;
  raw.setLevel(invaderz::log::Severity::DEBUG);
  invaderz::log::Locator::provide(&raw);

  invaderz::Game game;
  invaderz::App app(640, 480);

  bool running = true;
  while (running)
  {
    auto events = app.pollEvents();
    running     = game.update(events);
    game.render(app);
    app.render();
  }

  return EXIT_SUCCESS;
}

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

  invaderz::App app(640, 480);
  invaderz::Game game;

  bool running = true;
  while (running)
  {
    game.update();
    running = app.doFrame();
  }

  return EXIT_SUCCESS;
}
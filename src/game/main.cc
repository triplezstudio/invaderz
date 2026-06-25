
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

  constexpr auto width  = 480;
  constexpr auto height = 880;

  invaderz::Game game(Eigen::Vector3f(1.0f * width, 1.0f * height, 0.0f));
  invaderz::App app(width, height);

  bool running = true;
  while (running)
  {
    auto events = app.pollEvents();
    running     = game.update(events);
    app.clear();
    game.render(app);
    app.render();
  }

  return EXIT_SUCCESS;
}
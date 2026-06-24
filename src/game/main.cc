
#include "App.hh"
#include "Locator.hh"
#include "StdLogger.hh"
#include <iostream>

int main(int /*argc*/, char * /*argv*/[])
{
  invaderz::log::StdLogger raw;
  raw.setLevel(invaderz::log::Severity::DEBUG);
  invaderz::log::Locator::provide(&raw);

  invaderz::App app;

  while (true)
  {
    app.render();
  }

  return EXIT_SUCCESS;
}
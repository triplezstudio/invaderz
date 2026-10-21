
#include "AudioEngine.hh"
#include "Game.hh"
#include "Locator.hh"
#include "Renderer.hh"
#include "SdlAudioRegistry.hh"
#include "SdlFontRegistry.hh"
#include "SdlTextureRegistry.hh"
#include "StageFactory.hh"
#include "StdLogger.hh"
#include "Window.hh"

int main(int /*argc*/, char * /*argv*/[])
{
  invaderz::log::StdLogger raw;
  raw.setLevel(invaderz::log::Severity::DEBUG);
  invaderz::log::Locator::provide(&raw);

  constexpr auto width  = 480;
  constexpr auto height = 880;
  Eigen::Vector3f screenDims(1.0f * width, 1.0f * height, 0.0f);

  invaderz::Window window(480, 880, "invaderz");

  invaderz::IRendererPtr renderer = window.createRenderer();
  invaderz::AudioEngine audioEngine;

  invaderz::StageFactory factory(audioEngine.getAudioRegistry(),
                                 renderer->getFontRegistry(),
                                 renderer->getTextureRegistry(),
                                 screenDims);
  invaderz::Game game(std::move(factory));

  bool running = true;
  while (running)
  {
    auto events = window.pollEvents();
    running     = game.update(events);

    audioEngine.update();
    game.processSounds(audioEngine);

    renderer->clear();
    game.render(*renderer);
    renderer->render();
  }

  return EXIT_SUCCESS;
}
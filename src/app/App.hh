
#pragma once

#include "CoreObject.hh"
#include <SDL3/SDL.h>

namespace invaderz {

class App : public runtime::CoreObject
{
  public:
  App(const int width, const int height);
  ~App() override;

  /// @brief - Call once per frame, performs rendering, input management and returns whether the
  /// app should continue executing or not.
  /// @return - true if the app should continue running
  bool doFrame();

  private:
  SDL_Window *m_window{nullptr};
  SDL_Renderer *m_renderer{nullptr};

  void initializeSdl(const int width, const int height);

  bool processInput();
  void render();
};

} // namespace invaderz

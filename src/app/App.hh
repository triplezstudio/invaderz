
#pragma once

#include "CoreObject.hh"
#include "EventData.hh"
#include "IAudio.hh"
#include "IRenderer.hh"
#include <SDL3/SDL.h>

namespace invaderz {

class App : public runtime::CoreObject, public IRenderer
{
  public:
  App(const int width, const int height);
  ~App() override;

  auto pollEvents() -> EventData;

  void clear();
  void render();

  void renderRectangle(const Eigen::Vector3f &position, const Eigen::Vector3f &dims) override;

  private:
  SDL_Window *m_window{nullptr};
  SDL_Renderer *m_renderer{nullptr};

  void initializeSdl(const int width, const int height);
};

} // namespace invaderz

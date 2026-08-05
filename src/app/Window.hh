
#pragma once

#include "CoreObject.hh"
#include "FrameData.hh"
#include "IRenderer.hh"
#include "ITextureLoader.hh"
#include "InputState.hh"
#include "Timer.hh"

namespace invaderz {

class Window : public runtime::CoreObject
{
  public:
  Window(const int width, const int height, const std::string_view title);
  ~Window() override;

  auto getWidth() const -> int;
  auto getHeight() const -> int;

  auto pollEvents() -> FrameData;

  auto createRenderer() const -> IRendererPtr;

  private:
  InputState m_state{};
  Timer m_timer{};

  SDL_Window *m_window{};
  SDL_Renderer *m_renderer{};

  void initialize(const int width, const int height, const std::string_view title);
};

} // namespace invaderz

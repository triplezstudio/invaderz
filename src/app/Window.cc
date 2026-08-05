
#include "Window.hh"
#include "SdlException.hh"

namespace invaderz {

Window::Window(const int width, const int height, const std::string_view title)
  : runtime::CoreObject("window")
{
  initialize(width, height, title);
}

Window::~Window()
{
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}

auto Window::getWidth() const -> int
{
  int w, h;
  SDL_GetRenderOutputSize(m_renderer, &w, &h);
  return w;
}

auto Window::getHeight() const -> int
{
  int w, h;
  SDL_GetRenderOutputSize(m_renderer, &w, &h);
  return h;
}

auto Window::pollEvents() -> FrameData
{
  bool quit{false};
  SDL_Event event{};
  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_EVENT_QUIT)
    {
      quit = true;
    }
  }

  m_state.update();

  return FrameData{
    .quit    = quit,
    .state   = m_state,
    .elapsed = m_timer.restart(),
  };
}

auto Window::createRenderer() const -> IRendererPtr
{
  auto registry = std::make_unique<invaderz::SdlTextureRegistry>(m_renderer);
  return std::unique_ptr<Renderer>(new Renderer(m_renderer, std::move(registry)));
}

void Window::initialize(const int width, const int height, const std::string_view title)
{
  if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_AUDIO))
  {
    throw runtime::SdlException("Failed to initialize SDL");
  }

  if (!SDL_CreateWindowAndRenderer(title.data(), width, height, 0, &m_window, &m_renderer))
  {
    throw runtime::SdlException("Failed to initialize window/renderer");
  }
}

} // namespace invaderz

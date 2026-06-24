
#include "App.hh"

namespace invaderz {

constexpr auto WINDOW_TITLE = "invaderz";

App::App(const int width, const int height)
  : runtime::CoreObject("app")
{
  initializeSdl(width, height);
}

App::~App()
{
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}

auto App::pollEvents() -> EventData
{
  EventData data{};

  SDL_Event event{};

  while (SDL_PollEvent(&event))
  {
    data.events.push_back(event);
  }

  return data;
}

void App::render()
{
  SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
  SDL_RenderClear(m_renderer);
  SDL_RenderPresent(m_renderer);
}

void App::initializeSdl(const int width, const int height)
{
  if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO))
  {
    error("Failed to initialize SDL", std::string(SDL_GetError()));
  }

  if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, width, height, 0, &m_window, &m_renderer))
  {
    error("Failed to initialize window/renderer", std::string(SDL_GetError()));
  }
}

} // namespace invaderz

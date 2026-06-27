
#include "App.hh"
#include "SdlException.hh"

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

void App::clear()
{
  SDL_SetRenderDrawColor(m_renderer, 32, 32, 32, 255);
  SDL_RenderClear(m_renderer);
}

void App::render()
{
  SDL_RenderPresent(m_renderer);
}

void App::renderRectangle(const Eigen::Vector3f &position, const Eigen::Vector3f &dims)
{
  SDL_SetRenderDrawColor(m_renderer, 168, 119, 50, 255);
  SDL_FRect rect{
    .x = position(0),
    .y = position(1),
    .w = dims(0),
    .h = dims(1),
  };
  SDL_RenderFillRect(m_renderer, &rect);
}

void App::initializeSdl(const int width, const int height)
{
  if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_AUDIO))
  {
    throw runtime::SdlException("Failed to initialize SDL");
  }

  if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, width, height, 0, &m_window, &m_renderer))
  {
    throw runtime::SdlException("Failed to initialize window/renderer");
  }
}

} // namespace invaderz

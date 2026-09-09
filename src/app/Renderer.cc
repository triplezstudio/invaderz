
#include "Renderer.hh"
#include "SdlException.hh"

namespace invaderz {

Renderer::Renderer(SDL_Renderer *renderer, ITextureRegistryPtr registry)
  : m_renderer(std::move(renderer))
  , m_registry(std::move(registry))
{
  if (m_renderer == nullptr)
  {
    throw std::invalid_argument("Expected non null renderer");
  }
  if (m_registry == nullptr)
  {
    throw std::invalid_argument("Expected non null registry");
  }

  createTextEngine();
}

Renderer::~Renderer()
{
  TTF_DestroyRendererTextEngine(m_textEngine);
}

auto Renderer::getTextureRegistry() const -> ITextureRegistry &
{
  return *m_registry;
}

void Renderer::clear()
{
  SDL_SetRenderDrawColor(m_renderer, 32, 32, 32, 255);
  SDL_RenderClear(m_renderer);
}

void Renderer::render()
{
  SDL_RenderPresent(m_renderer);
}

namespace {
constexpr std::uint8_t ALPHA_OPAQUE = 255;

auto colorToRgb(const Color color) -> std::array<std::uint8_t, 3>
{
  switch (color)
  {
    case Color::ORANGE:
      return std::array<std::uint8_t, 3>{168, 119, 50};
    case Color::TURQUOISE:
      return std::array<std::uint8_t, 3>{90, 222, 209};
    case Color::BURGUNDY:
      return std::array<std::uint8_t, 3>{102, 0, 51};
    default:
      throw std::runtime_error("Unsupported color");
  }
}
} // namespace

void Renderer::renderRectangle(const Eigen::Vector3f &position,
                               const Eigen::Vector3f &dims,
                               const Color color)
{
  auto rgb = colorToRgb(color);

  SDL_SetRenderDrawColor(m_renderer, rgb[0], rgb[1], rgb[2], ALPHA_OPAQUE);
  SDL_FRect rect{
    .x = position(0),
    .y = position(1),
    .w = dims(0),
    .h = dims(1),
  };
  SDL_RenderFillRect(m_renderer, &rect);
}

void Renderer::renderTexture(const TextureId textureId,
                             const Eigen::Vector3f &position,
                             const Eigen::Vector3f &dims,
                             const float angle)
{
  auto &texture = m_registry->getTexture(textureId);

  SDL_FRect rect{
    .x = position(0),
    .y = position(1),
    .w = dims(0),
    .h = dims(1),
  };
  SDL_RenderTextureRotated(m_renderer,
                           texture.getRaw(),
                           nullptr,
                           &rect,
                           angle,
                           nullptr,
                           SDL_FLIP_NONE);
}

void Renderer::renderText(const std::string &text)
{
  SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
  SDL_RenderDebugText(m_renderer, 10, 10, text.c_str());
  SDL_SetRenderDrawColor(m_renderer, 32, 32, 32, 255);
}

void Renderer::createTextEngine()
{
  m_textEngine = TTF_CreateRendererTextEngine(m_renderer);
  if (m_textEngine == nullptr)
  {
    throw runtime::SdlException("Failed to initialize text renderer");
  }
}

} // namespace invaderz

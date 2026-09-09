
#include "Renderer.hh"
#include "SdlException.hh"
#include <format>

namespace invaderz {

Renderer::Renderer(SDL_Renderer *renderer,
                   ITextureRegistryPtr textureRegistry,
                   IFontRegistryPtr fontRegistry)
  : m_renderer(std::move(renderer))
  , m_textureRegistry(std::move(textureRegistry))
  , m_fontRegistry(std::move(fontRegistry))
{
  if (m_renderer == nullptr)
  {
    throw std::invalid_argument("Expected non null renderer");
  }
  if (m_textureRegistry == nullptr)
  {
    throw std::invalid_argument("Expected non null texture registry");
  }
  if (m_fontRegistry == nullptr)
  {
    throw std::invalid_argument("Expected non null font registry");
  }

  createTextEngine();
}

Renderer::~Renderer()
{
  TTF_DestroyRendererTextEngine(m_textEngine);
}

auto Renderer::getTextureRegistry() const -> ITextureRegistry &
{
  return *m_textureRegistry;
}

auto Renderer::getFontRegistry() const -> IFontRegistry &
{
  return *m_fontRegistry;
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
  auto &texture = m_textureRegistry->getTexture(textureId);

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

namespace {
TTF_Font *font = nullptr;
std::unordered_map<std::string, TTF_Text *> textCache{};
constexpr auto FONT_SIZE = 30;

auto createText(TTF_TextEngine *engine, const std::string &text) -> TTF_Text *
{
  const auto maybeText = textCache.find(text);
  if (maybeText != textCache.end())
  {
    return maybeText->second;
  }

  if (font == nullptr)
  {
    auto filePath = std::format("{}/ArcadeClassic.ttf", std::getenv("ASSET_FOLDER"));
    font          = TTF_OpenFont(filePath.c_str(), FONT_SIZE);
  }

  auto ttfText    = TTF_CreateText(engine, font, text.c_str(), text.size());
  textCache[text] = ttfText;
  return ttfText;
}
} // namespace

void Renderer::renderText(const FontId fontId,
                          const std::string &text,
                          const Eigen::Vector3f &position)
{
  /*auto &font =*/m_fontRegistry->getFont(fontId);

  auto gpuText = createText(m_textEngine, text);
  TTF_DrawRendererText(gpuText, static_cast<int>(position(0)), static_cast<int>(position(1)));

  // SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
  // SDL_RenderDebugText(m_renderer, 10, 10, text.c_str());
  // SDL_SetRenderDrawColor(m_renderer, 32, 32, 32, 255);
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


#include "Texture.hh"
#include "SdlException.hh"

namespace invaderz::assets {
namespace {
auto loadTexture(const std::string_view filePath) -> SDL_Texture *
{
  auto surface = SDL_LoadPNG(filePath.data());
  if (surface == nullptr)
  {
    throw runtime::SdlException("Couldn't load .png file \"" + std::string(filePath) + "\"");
  }

  auto texture = SDL_CreateTextureFromSurface(nullptr, surface);
  if (texture == nullptr)
  {
    throw runtime::SdlException("Failed to created texture");
  }

  return texture;
}
} // namespace

Texture::Texture(const std::string_view filePath)
  : m_texture(loadTexture(filePath))
{}

Texture::~Texture()
{
  SDL_DestroyTexture(m_texture);
}

} // namespace invaderz::assets

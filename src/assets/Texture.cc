
#include "Texture.hh"
#include "SdlException.hh"

#include <iostream>

namespace invaderz::assets {

Texture::Texture(const std::string_view filePath, ITextureLoader *loader)
{
  auto surface = SDL_LoadPNG(filePath.data());
  if (surface == nullptr)
  {
    throw runtime::SdlException("Couldn't load .png file \"" + std::string(filePath) + "\"");
  }

  m_texture = SDL_CreateTextureFromSurface(loader, surface);

  if (m_texture == nullptr)
  {
    throw runtime::SdlException("Failed to created texture");
  }
}

auto Texture::getRaw() const -> SDL_Texture *
{
  return m_texture;
}

Texture::~Texture()
{
  SDL_DestroyTexture(m_texture);
}

} // namespace invaderz::assets

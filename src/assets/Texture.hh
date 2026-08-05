
#pragma once

#include "ITextureLoader.hh"
#include <memory>
#include <string_view>

namespace invaderz::assets {

class Texture
{
  public:
  Texture(const std::string_view filePath, ITextureLoader *loader);
  ~Texture();

  auto getRaw() const -> SDL_Texture *;

  private:
  SDL_Texture *m_texture{};
};

using TexturePtr = std::unique_ptr<Texture>;

} // namespace invaderz::assets

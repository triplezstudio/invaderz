
#pragma once

#include <SDL3/SDL.h>
#include <memory>
#include <string_view>

namespace invaderz::assets {

class Texture
{
  public:
  Texture(const std::string_view filePath);
  ~Texture();

  private:
  SDL_Texture *m_texture{nullptr};
};

using TexturePtr = std::unique_ptr<Texture>;

} // namespace invaderz::assets

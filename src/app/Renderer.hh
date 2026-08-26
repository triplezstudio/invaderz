
#pragma once

#include "Color.hh"
#include "ITextureRegistry.hh"
#include <SDL3/SDL.h>
#include <eigen3/Eigen/Eigen>

namespace invaderz {

class Window;

class Renderer
{
  public:
  virtual ~Renderer() = default;

  auto getTextureRegistry() const -> ITextureRegistry &;

  void clear();
  void render();

  void renderRectangle(const Eigen::Vector3f &position,
                       const Eigen::Vector3f &dims,
                       const Color color);

  void renderTexture(const TextureId textureId,
                     const Eigen::Vector3f &position,
                     const Eigen::Vector3f &dims,
                     const float angle = 0.0f);

  private:
  friend class Window;

  Renderer(SDL_Renderer *renderer, ITextureRegistryPtr registry);

  SDL_Renderer *m_renderer{nullptr};
  ITextureRegistryPtr m_registry{};
};

} // namespace invaderz

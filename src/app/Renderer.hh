
#pragma once

#include "Color.hh"
#include "ITextureRegistry.hh"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <eigen3/Eigen/Eigen>

namespace invaderz {

class Window;

class Renderer
{
  public:
  virtual ~Renderer();

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

  void renderText(const std::string &text);

  private:
  friend class Window;

  Renderer(SDL_Renderer *renderer, ITextureRegistryPtr registry);

  SDL_Renderer *m_renderer{nullptr};
  TTF_TextEngine *m_textEngine{nullptr};
  ITextureRegistryPtr m_registry{};

  void createTextEngine();
};

} // namespace invaderz

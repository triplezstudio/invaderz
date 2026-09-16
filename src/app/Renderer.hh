
#pragma once

#include "Color.hh"
#include "IFontRegistry.hh"
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
  auto getFontRegistry() const -> IFontRegistry &;

  void clear();
  void render();

  void renderRectangle(const Eigen::Vector3f &position,
                       const Eigen::Vector3f &dims,
                       const Color color);

  void renderTexture(const TextureId textureId,
                     const Eigen::Vector3f &position,
                     const Eigen::Vector3f &dims,
                     const float angle = 0.0f);

  void renderPartialTexture(const TextureId textureId,
                            const Eigen::Vector3f &src,
                            const Eigen::Vector3f &srcDims,
                            const Eigen::Vector3f &dst,
                            const Eigen::Vector3f &dstDims);

  void renderText(const FontId fontId, const std::string &text, const Eigen::Vector3f &position);

  private:
  friend class Window;

  Renderer(SDL_Renderer *renderer,
           ITextureRegistryPtr textureRegistry,
           IFontRegistryPtr fontRegistry);

  SDL_Renderer *m_renderer{nullptr};
  ITextureRegistryPtr m_textureRegistry{};
  IFontRegistryPtr m_fontRegistry{};
};

} // namespace invaderz

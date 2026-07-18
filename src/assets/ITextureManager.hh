
#pragma once

#include "Asset.hh"
#include "Texture.hh"
#include <memory>
#include <string_view>

namespace invaderz {

using TextureId = int;

class ITextureManager
{
  public:
  ITextureManager()          = default;
  virtual ~ITextureManager() = default;

  /// @brief - Registers a new texture file as a usable asset in the project.
  /// The texture file is expected to have a PNG format: anything else will
  /// lead to undefined behavior.
  /// Once loaded, the texture is available under the returned identifier.
  /// @param filePath - the location of the resource on the filesystem
  /// @return - an identifier for the texture
  virtual auto registerTexture(const std::string_view filePath) -> assets::Asset = 0;

  virtual auto getTexture(const TextureId id) const -> assets::Texture & = 0;
};

using ITextureManagerPtr = std::unique_ptr<ITextureManager>;

} // namespace invaderz

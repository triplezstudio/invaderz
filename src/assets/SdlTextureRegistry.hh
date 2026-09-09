
#pragma once

#include "CoreObject.hh"
#include "Texture.hh"
#include <atomic>
#include <memory>
#include <string_view>
#include <unordered_map>

namespace invaderz {

using TextureId = int;

class SdlTextureRegistry : public runtime::CoreObject
{
  public:
  SdlTextureRegistry(ITextureLoader *loader);
  virtual ~SdlTextureRegistry() = default;

  auto registerTexture(const std::string_view filePath) -> TextureId;
  void unregister(const TextureId &texture);

  auto getTexture(const TextureId id) const -> assets::Texture &;

  private:
  ITextureLoader *m_loader{};

  std::atomic<TextureId> m_nextId{0};
  std::unordered_map<TextureId, assets::TexturePtr> m_textures{};
};

using SdlTextureRegistryPtr = std::unique_ptr<SdlTextureRegistry>;

} // namespace invaderz

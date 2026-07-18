
#pragma once

#include "CoreObject.hh"
#include "IAssetManager.hh"
#include "IAudioManager.hh"
#include "ITextureManager.hh"
#include <atomic>
#include <unordered_map>

namespace invaderz {

class SdlAssetManager : public IAudioManager,
                        public ITextureManager,
                        public IAssetManager,
                        public runtime::CoreObject
{
  public:
  SdlAssetManager();
  ~SdlAssetManager() override;

  auto registerSound(const std::string_view filePath) -> assets::Asset override;
  auto getSound(const SoundId id) const -> assets::Sound & override;

  auto registerTexture(const std::string_view filePath) -> assets::Asset override;
  auto getTexture(const TextureId id) const -> assets::Texture & override;

  void unregister(const assets::Asset &asset) override;

  private:
  std::atomic<assets::Id> m_nextAssetId{0};
  std::unordered_map<SoundId, assets::SoundPtr> m_sounds{};
  std::unordered_map<TextureId, assets::TexturePtr> m_textures{};
};

} // namespace invaderz

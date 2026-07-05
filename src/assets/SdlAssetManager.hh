
#pragma once

#include "CoreObject.hh"
#include "IAudioManager.hh"
#include "Sound.hh"
#include <atomic>
#include <unordered_map>

namespace invaderz {

class SdlAssetManager : public IAudioManager, public runtime::CoreObject
{
  public:
  SdlAssetManager();
  ~SdlAssetManager() override;

  auto registerSound(const std::string_view filePath) -> assets::Asset override;
  void unregister(const assets::Asset &asset) override;

  auto getSound(const SoundId id) const -> assets::Sound & override;

  private:
  std::atomic<assets::Id> m_nextAssetId{0};
  std::unordered_map<SoundId, assets::SoundPtr> m_sounds{};
};

} // namespace invaderz

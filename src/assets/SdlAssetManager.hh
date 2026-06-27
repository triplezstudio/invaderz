
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

  void playOnce(const SoundId id, const float volume) override;

  void update() override;

  private:
  enum class Mode
  {
    ONCE,
  };
  struct PlayingSound
  {
    assets::Id id{};
    Mode mode{Mode::ONCE};
  };

  SDL_AudioDeviceID m_audioDeviceId{0};

  std::atomic<assets::Id> m_nextAssetId{0};
  std::unordered_map<SoundId, assets::SoundDataPtr> m_sounds{};
  std::vector<PlayingSound> m_currentlyPlayingSounds{};

  void initialize();
  void updatePlayingSound(const PlayingSound &sound);
};

} // namespace invaderz

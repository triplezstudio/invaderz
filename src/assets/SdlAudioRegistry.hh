
#pragma once

#include "CoreObject.hh"
#include "Sound.hh"
#include <atomic>
#include <memory>
#include <string_view>
#include <unordered_map>

namespace invaderz {

using SoundId = int;

class SdlAudioRegistry : public runtime::CoreObject
{
  public:
  SdlAudioRegistry();
  virtual ~SdlAudioRegistry() = default;

  /// @brief - Registers a new audio file as a usable asset in the project.
  /// The audio file is expected to have a WAV format: anything else will
  /// lead to undefined behavior.
  /// Once loaded, the sound is available under the returned identifier.
  /// @param filePath - the location of the resource on the filesystem
  /// @return - an identifier for the sound
  auto registerSound(const std::string_view filePath) -> SoundId;
  void unregister(const SoundId &sound);

  auto getSound(const SoundId id) const -> assets::Sound &;

  private:
  std::atomic<SoundId> m_nextId{0};
  std::unordered_map<SoundId, assets::SoundPtr> m_sounds{};
};

using SdlAudioRegistryPtr = std::unique_ptr<SdlAudioRegistry>;

} // namespace invaderz

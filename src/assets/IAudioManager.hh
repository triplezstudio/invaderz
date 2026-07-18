
#pragma once

#include "Asset.hh"
#include "Sound.hh"
#include <memory>
#include <string_view>

namespace invaderz {

using SoundId = int;

class IAudioManager
{
  public:
  IAudioManager()          = default;
  virtual ~IAudioManager() = default;

  /// @brief - Registers a new audio file as a usable asset in the project.
  /// The audio file is expected to have a WAV format: anything else will
  /// lead to undefined behavior.
  /// Once loaded, the sound is available under the returned identifier.
  /// @param filePath - the location of the resource on the filesystem
  /// @return - an identifier for the sound
  virtual auto registerSound(const std::string_view filePath) -> assets::Asset = 0;

  virtual auto getSound(const SoundId id) const -> assets::Sound & = 0;
};

using IAudioManagerPtr = std::unique_ptr<IAudioManager>;

} // namespace invaderz


#pragma once

#include "Asset.hh"
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
  virtual void unregister(const assets::Asset &asset)                          = 0;

  virtual void playOnce(const SoundId id, const float volume) = 0;

  /// @brief - Used to update the currently playing sounds: it also cleans
  /// sounds which are terminated.
  virtual void update() = 0;
};

} // namespace invaderz

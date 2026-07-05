
#pragma once

#include "IAudioManager.hh"

namespace invaderz {

class IAudioEngine
{
  public:
  IAudioEngine()          = default;
  virtual ~IAudioEngine() = default;

  virtual void playOnce(const SoundId id, const float volume) = 0;

  /// @brief - Used to update the currently playing sounds: it also cleans
  /// sounds which are terminated.
  virtual void update() = 0;
};

} // namespace invaderz

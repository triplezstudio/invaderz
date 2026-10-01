
#pragma once

#include "CoreObject.hh"
#include "IAudioRegistry.hh"
#include "PlayingSound.hh"
#include <vector>

namespace invaderz {

class AudioEngine : public runtime::CoreObject
{
  public:
  AudioEngine();
  virtual ~AudioEngine();

  auto getAudioRegistry() const -> IAudioRegistry &;

  void playOnce(const SoundId id, const float volume);

  /// @brief - Used to update the currently playing sounds: it also cleans
  /// sounds which are terminated.
  void update();

  private:
  SDL_AudioDeviceID m_audioDeviceId{0};
  MIX_Mixer *m_mixer{};
  IAudioRegistryPtr m_registry{};

  std::vector<PlayingSoundPtr> m_currentlyPlayingSounds{};

  void initializeAudio();
};

} // namespace invaderz

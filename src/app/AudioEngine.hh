
#pragma once

#include "CoreObject.hh"
#include "IAudioRegistry.hh"
#include <vector>

namespace invaderz {

class AudioEngine : public runtime::CoreObject
{
  public:
  AudioEngine(IAudioRegistryPtr registry);
  virtual ~AudioEngine();

  void playOnce(const SoundId id, const float volume);

  /// @brief - Used to update the currently playing sounds: it also cleans
  /// sounds which are terminated.
  void update();

  private:
  SDL_AudioDeviceID m_audioDeviceId{0};
  IAudioRegistryPtr m_registry{};

  // TODO: handle "looping"
  enum class Mode
  {
    ONCE,
  };
  struct PlayingSound
  {
    SoundId id{};
    Mode mode{Mode::ONCE};
  };

  std::vector<PlayingSound> m_currentlyPlayingSounds{};

  void initializeAudio();

  void updatePlayingSound(const PlayingSound &sound);
};

} // namespace invaderz

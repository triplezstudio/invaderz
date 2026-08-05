
#include "AudioEngine.hh"
#include "SdlException.hh"

namespace invaderz {

AudioEngine::AudioEngine(IAudioRegistryPtr registry)
  : runtime::CoreObject("audio")
  , m_registry(std::move(registry))
{
  if (m_registry == nullptr)
  {
    throw std::invalid_argument("Expected non null registry");
  }

  initializeAudio();
}

AudioEngine::~AudioEngine()
{
  m_registry.reset();
  SDL_CloseAudioDevice(m_audioDeviceId);
}

void AudioEngine::playOnce(const SoundId id, const float volume)
{
  auto &sound = m_registry->getSound(id);

  sound.bindToAudioDevice(m_audioDeviceId, volume);
  m_currentlyPlayingSounds.push_back(PlayingSound{.id = id, .mode = Mode::ONCE});
}

void AudioEngine::update()
{
  for (const auto &sound : m_currentlyPlayingSounds)
  {
    updatePlayingSound(sound);
  }

  std::erase_if(m_currentlyPlayingSounds, [this](const PlayingSound &sound) {
    auto &soundData = m_registry->getSound(sound.id);
    return soundData.isFinished();
  });
}

void AudioEngine::initializeAudio()
{
  SDL_AudioSpec want{
    .format   = SDL_AUDIO_F32,
    .channels = 2,
    .freq     = 48000,
  };

  m_audioDeviceId = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &want);
  if (m_audioDeviceId == 0)
  {
    throw runtime::SdlException("Failed to open audio device");
  }

  info(std::string("Bound to audio device ") + SDL_GetAudioDeviceName(m_audioDeviceId));
}

void AudioEngine::updatePlayingSound(const PlayingSound &sound)
{
  auto &soundData = m_registry->getSound(sound.id);

  if (!soundData.isFinished())
  {
    soundData.update();
  }
}

} // namespace invaderz

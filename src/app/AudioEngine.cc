
#include "AudioEngine.hh"
#include "SdlException.hh"

namespace invaderz {

AudioEngine::AudioEngine()
  : runtime::CoreObject("audio")
{
  initializeAudio();
}

AudioEngine::~AudioEngine()
{
  m_registry.reset();
  SDL_CloseAudioDevice(m_audioDeviceId);
}

auto AudioEngine::getAudioRegistry() const -> IAudioRegistry &
{
  return *m_registry;
}

void AudioEngine::playOnce(const SoundId id, const float volume)
{
  auto &sound = m_registry->getSound(id);
  m_currentlyPlayingSounds.push_back(
    std::make_unique<PlayingSound>(sound, m_mixer, Mode::ONCE, volume));
}

void AudioEngine::update()
{
  for (const auto &sound : m_currentlyPlayingSounds)
  {
    sound->update();
  }

  std::erase_if(m_currentlyPlayingSounds,
                [](const PlayingSoundPtr &sound) { return sound->isFinished(); });
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

  m_mixer = MIX_CreateMixerDevice(m_audioDeviceId, nullptr);
  if (m_mixer == nullptr)
  {
    throw runtime::SdlException("Failed to create audio mixer");
  }

  m_registry = std::make_unique<invaderz::SdlAudioRegistry>(m_mixer);

  info(std::string("Bound to audio device ") + SDL_GetAudioDeviceName(m_audioDeviceId));
}

} // namespace invaderz

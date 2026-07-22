
#include "Sound.hh"
#include "SdlException.hh"

namespace invaderz::assets {

Sound::Sound(const std::string_view filePath)
{
  SDL_AudioSpec spec;
  uint32_t size{};

  if (!SDL_LoadWAV(filePath.data(), &spec, &m_data, &size))
  {
    throw runtime::SdlException("Couldn't load .wav file \"" + std::string(filePath) + "\"");
  }

  m_lengthInBytes = static_cast<int>(size);

  m_stream = SDL_CreateAudioStream(&spec, nullptr);
  if (!m_stream)
  {
    throw runtime::SdlException("Failed to create audio stream for \"" + std::string(filePath)
                                + "\"");
  }
}

Sound::~Sound()
{
  SDL_DestroyAudioStream(m_stream);
}

bool Sound::isFinished() const
{
  return SDL_GetAudioStreamQueued(m_stream) >= m_lengthInBytes;
}

void Sound::update()
{
  // TODO: This puts the whole data of the sound in the playback queue.
  // This means that the sound is 'finished' immediately after the first
  // update.
  // This means that we cannot pause/stop it because the data has already
  // been sent to the audio card.
  // To solve this, we would:
  //  - either need to have several logical devices
  //      https://wiki.libsdl.org/SDL3/SDL_PauseAudioStreamDevice
  //      https://wiki.libsdl.org/SDL3/SDL_OpenAudioDevice
  //  - or precisely put as many bytes as needed to be slightly 'ahead' of
  //    the playback.
  // Calculating the number of bytes to put is a function of the frequency
  // of the sound, the number of channels and the format of the sound.
  SDL_PutAudioStreamData(m_stream, m_data, m_lengthInBytes);
}

void Sound::bindToAudioDevice(const int deviceId, const float volume)
{
  SDL_BindAudioStream(deviceId, m_stream);
  SDL_SetAudioStreamGain(m_stream, volume);
}

} // namespace invaderz::assets

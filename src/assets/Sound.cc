
#include "Sound.hh"
#include "SdlException.hh"

#include <iostream>

namespace invaderz::assets {

SoundData::SoundData(const std::string_view filePath)
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

SoundData::~SoundData()
{
  SDL_DestroyAudioStream(m_stream);
}

bool SoundData::isFinished() const
{
  return SDL_GetAudioStreamQueued(m_stream) >= m_lengthInBytes;
}

void SoundData::update()
{
  SDL_PutAudioStreamData(m_stream, m_data, m_lengthInBytes);
}

void SoundData::bindToAudioDevice(const int deviceId, const float volume)
{
  SDL_BindAudioStream(deviceId, m_stream);
  SDL_SetAudioStreamGain(m_stream, volume);
}

} // namespace invaderz::assets

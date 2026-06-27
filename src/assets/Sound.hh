
#pragma once

#include <SDL3/SDL.h>
#include <memory>
#include <string_view>

namespace invaderz::assets {

class SoundData
{
  public:
  SoundData(const std::string_view filePath);
  ~SoundData();

  bool isFinished() const;
  void update();
  void bindToAudioDevice(const int deviceId, const float volume);

  private:
  SDL_AudioStream *m_stream{nullptr};
  uint8_t *m_data{nullptr};
  int m_lengthInBytes{0};
};

using SoundDataPtr = std::unique_ptr<SoundData>;

} // namespace invaderz::assets

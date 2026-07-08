
#pragma once

#include <SDL3/SDL.h>
#include <memory>
#include <string_view>

namespace invaderz::assets {

class Sound
{
  public:
  Sound(const std::string_view filePath);
  ~Sound();

  bool isFinished() const;
  void update();
  void bindToAudioDevice(const int deviceId, const float volume);

  private:
  SDL_AudioStream *m_stream{nullptr};
  uint8_t *m_data{nullptr};
  int m_lengthInBytes{0};
};

using SoundPtr = std::unique_ptr<Sound>;

} // namespace invaderz::assets

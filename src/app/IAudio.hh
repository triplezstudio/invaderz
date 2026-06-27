//
// Created by mgrus on 25.06.2026.
//

#ifndef INVADERZ_IAUDIO_H
#define INVADERZ_IAUDIO_H

#include <memory>
#include <string>

struct WaveData
{
  SDL_AudioStream *stream = nullptr;
  uint8_t *data           = nullptr;
  uint32_t lengthInBytes  = 0;
};

class IAudio
{
  public:
  IAudio()          = default;
  virtual ~IAudio() = default;

  virtual std::unique_ptr<WaveData> loadWavFile(const std::string &filePath) = 0;
  virtual void playSound(WaveData *waveData, float volume)                   = 0;
};

#endif //INVADERZ_IAUDIO_H

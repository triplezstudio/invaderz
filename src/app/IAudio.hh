//
// Created by mgrus on 25.06.2026.
//

#ifndef INVADERZ_IAUDIO_H
#define INVADERZ_IAUDIO_H

#include <string>

struct WaveData
{
  SDL_AudioSpec spec;
  SDL_AudioStream *stream = NULL;
};

class IAudio
{
  public:
  IAudio() = default;
  virtual ~IAudio() = default;

  virtual std::unique_ptr<WaveData> loadWavFile(const std::string& filePath);
};

#endif //INVADERZ_IAUDIO_H

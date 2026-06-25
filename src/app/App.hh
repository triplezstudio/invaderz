
#pragma once

#include "CoreObject.hh"
#include "EventData.hh"
#include "IRenderer.hh"
#include "IAudio.hh"
#include <SDL3/SDL.h>

namespace invaderz {

class App : public runtime::CoreObject, public IRenderer, public IAudio
{
  public:
  App(const int width, const int height);
  ~App() override;

  auto pollEvents() -> EventData;

  void clear();
  void render();

  void renderRectangle(const Eigen::Vector3f &position, const Eigen::Vector3f &dims) override;


  std::unique_ptr<WaveData> loadWavFile(const std::string &filePath) override;
  void playSound(WaveData* waveData, float volume) override;

  private:
  SDL_Window *m_window{nullptr};
  SDL_Renderer *m_renderer{nullptr};
  SDL_AudioDeviceID audioDeviceId{0};

  std::vector<WaveData*> currentlyPlayingSounds;

  void initializeSdl(const int width, const int height);
};

} // namespace invaderz

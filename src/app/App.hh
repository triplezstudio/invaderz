
#pragma once

#include "CoreObject.hh"
#include "FrameData.hh"
#include "IAudioEngine.hh"
#include "IAudioManager.hh"
#include "IRenderer.hh"
#include "InputState.hh"
#include "Timer.hh"
#include <SDL3/SDL.h>

namespace invaderz {

class App : public runtime::CoreObject, public IRenderer, public IAudioEngine
{
  public:
  App(const int width, const int height, IAudioManagerPtr audioManager);
  ~App() override;

  auto pollEvents() -> FrameData;

  void clear();
  void render();

  void renderRectangle(const Eigen::Vector3f &position, const Eigen::Vector3f &dims) override;

  void playOnce(const SoundId id, const float volume) override;
  void update() override;

  private:
  SDL_Window *m_window{nullptr};
  SDL_Renderer *m_renderer{nullptr};
  SDL_AudioDeviceID m_audioDeviceId{0};
  IAudioManagerPtr m_audioManager{};
  InputState m_state{};
  Timer m_timer{};

  enum class Mode
  {
    ONCE,
  };
  struct PlayingSound
  {
    assets::Id id{};
    Mode mode{Mode::ONCE};
  };

  std::vector<PlayingSound> m_currentlyPlayingSounds{};

  void initializeSdl(const int width, const int height);
  void initializeAudio();
  void updatePlayingSound(const PlayingSound &sound);
};

} // namespace invaderz

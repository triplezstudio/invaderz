
#pragma once

#include "Sound.hh"
#include <memory>

namespace invaderz {

enum class Mode
{
  ONCE,
};

enum class PlaybackStatus
{
  UNKNOWN,
  PLAYING,
  PAUSED
};

class PlayingSound
{
  public:
  PlayingSound(assets::Sound &sound, ISoundLoader *loader, const Mode mode, const float volume);
  ~PlayingSound();

  bool isFinished() const;
  void update();

  private:
  MIX_Track *m_track{};
  Mode m_mode{Mode::ONCE};
  PlaybackStatus m_status{PlaybackStatus::UNKNOWN};
};

using PlayingSoundPtr = std::unique_ptr<PlayingSound>;

} // namespace invaderz

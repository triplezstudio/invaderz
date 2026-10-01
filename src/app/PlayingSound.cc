
#include "PlayingSound.hh"
#include "SdlException.hh"

namespace invaderz {

PlayingSound::PlayingSound(assets::Sound &sound,
                           ISoundLoader *loader,
                           const Mode mode,
                           const float volume)
  : m_mode(mode)
{
  m_track = MIX_CreateTrack(loader);
  if (m_track == nullptr)
  {
    throw runtime::SdlException("Failed to create audio track");
  }

  if (!MIX_SetTrackAudio(m_track, sound.getRaw()))
  {
    throw runtime::SdlException("Failed to configure audio track");
  }

  if (!MIX_SetTrackGain(m_track, volume))
  {
    throw runtime::SdlException("Failed to configure track gain");
  }
}

PlayingSound::~PlayingSound()
{
  MIX_DestroyTrack(m_track);
}

bool PlayingSound::isFinished() const
{
  return !MIX_TrackPlaying(m_track);
}

void PlayingSound::update()
{
  if (m_status == PlaybackStatus::UNKNOWN)
  {
    m_status = PlaybackStatus::PLAYING;
    MIX_PlayTrack(m_track, 0);
  }
}

} // namespace invaderz

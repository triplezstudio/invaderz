
#include "SdlAssetManager.hh"
#include "SdlException.hh"
#include <SDL3/SDL.h>

namespace invaderz {

SdlAssetManager::SdlAssetManager()
  : runtime::CoreObject("manager")
{
  addModule("assets");
  initialize();
}

SdlAssetManager::~SdlAssetManager()
{
  m_sounds.clear();
  SDL_CloseAudioDevice(m_audioDeviceId);
}

auto SdlAssetManager::registerSound(const std::string_view filePath) -> assets::Asset
{
  auto data = std::make_unique<assets::SoundData>(filePath);

  auto assetId = m_nextAssetId.fetch_add(1);
  auto asset   = assets::Asset{.type = assets::Type::SOUND, .id = assetId};

  m_sounds.emplace(assetId, std::move(data));

  return asset;
}

void SdlAssetManager::unregister(const assets::Asset &asset)
{
  if (!asset.valid())
  {
    return;
  }

  switch (asset.type)
  {
    case assets::Type::SOUND:
      m_sounds.erase(asset.id);
      break;
    default:
      error("Unsupported asset type " + str(asset.type));
  }
}

void SdlAssetManager::playOnce(const SoundId id, const float volume)
{
  const auto maybeSound = m_sounds.find(id);
  if (maybeSound == m_sounds.end())
  {
    error("Unknown sound " + std::to_string(id));
  }

  maybeSound->second->bindToAudioDevice(m_audioDeviceId, volume);
  m_currentlyPlayingSounds.push_back(PlayingSound{.id = id, .mode = Mode::ONCE});
}

void SdlAssetManager::update()
{
  for (const auto &sound : m_currentlyPlayingSounds)
  {
    updatePlayingSound(sound);
  }
}

void SdlAssetManager::initialize()
{
  SDL_AudioSpec want{
    .format   = SDL_AUDIO_F32,
    .channels = 2,
    .freq     = 48000,
  };

  m_audioDeviceId = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &want);
  if (m_audioDeviceId == 0)
  {
    throw runtime::SdlException("Failed to open audio device");
  }

  info(std::string("Bound to audio device ") + SDL_GetAudioDeviceName(m_audioDeviceId));
}

void SdlAssetManager::updatePlayingSound(const PlayingSound &sound)
{
  const auto maybeSound = m_sounds.find(sound.id);
  if (maybeSound == m_sounds.end())
  {
    error("Unknown playing sound " + std::to_string(sound.id));
  }

  assets::SoundData &soundData = *maybeSound->second;

  if (!soundData.isFinished())
  {
    soundData.update();
  }
  // TODO: remove a "finished" sound from our currently playing list?
  // TODO: handle "looping"
}

} // namespace invaderz
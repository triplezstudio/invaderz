
#include "App.hh"

namespace invaderz {

constexpr auto WINDOW_TITLE = "invaderz";

App::App(const int width, const int height)
  : runtime::CoreObject("app")
{
  initializeSdl(width, height);
}

App::~App()
{
  SDL_CloseAudioDevice(audioDeviceId);
  // TODO destroy loaded audio streams

  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}

auto App::pollEvents() -> EventData
{
  EventData data{};

  SDL_Event event{};

  while (SDL_PollEvent(&event))
  {
    data.events.push_back(event);
  }

  // Update all of our playing sounds as well
  for (auto waveData : currentlyPlayingSounds) {
    if (SDL_GetAudioStreamQueued(waveData->stream) < ((int) waveData->lengthInBytes)) {
      SDL_PutAudioStreamData(waveData->stream, waveData->data, (int) waveData->lengthInBytes);
    }
    else {
      // TODO: remove a "finished" sound from our currently playing list?
      // TODO: handle "looping"
    }
  }



  return data;
}

void App::clear()
{
  SDL_SetRenderDrawColor(m_renderer, 32, 32, 32, 255);
  SDL_RenderClear(m_renderer);
}

void App::render()
{
  SDL_RenderPresent(m_renderer);
}

void App::renderRectangle(const Eigen::Vector3f &position, const Eigen::Vector3f &dims)
{
  SDL_SetRenderDrawColor(m_renderer, 168, 119, 50, 255);
  SDL_FRect rect{
    .x = position(0),
    .y = position(1),
    .w = dims(0),
    .h = dims(1),
  };
  SDL_RenderFillRect(m_renderer, &rect);
}

void App::initializeSdl(const int width, const int height)
{
  if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_AUDIO))
  {
    error("Failed to initialize SDL", std::string(SDL_GetError()));
  }

  if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, width, height, 0, &m_window, &m_renderer))
  {
    error("Failed to initialize window/renderer", std::string(SDL_GetError()));
  }

  // Audio setup
  SDL_AudioSpec want;

  SDL_memset(&want, 0, sizeof(want)); /* or SDL_zero(want) */
  want.format = SDL_AUDIO_F32;
  want.channels = 2;
  want.freq = 48000;

  audioDeviceId = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &want);
  if (audioDeviceId == 0) {
    error("Failed to open audio: %s", SDL_GetError());
  } else {
    info(std::string("Bound to audio device ")+ SDL_GetAudioDeviceName(audioDeviceId));
  }

}
std::unique_ptr<WaveData> App::loadWavFile(const std::string &filePath)
{
  SDL_AudioSpec spec;
  uint8_t *wavData      = NULL;
  uint32_t wavDataLength = 0;

  if (!SDL_LoadWAV(filePath.c_str(), &spec, &wavData, &wavDataLength)) {
    error("Couldn't load .wav file: %s", SDL_GetError());
  }

  auto stream = SDL_CreateAudioStream(&spec, NULL);
  if (!stream)
  {
    error("Audio stream could not be created! %s", SDL_GetError());
  }

  WaveData wd= {};
  wd.stream = stream;
  wd.data = wavData;
  wd.lengthInBytes = wavDataLength;
  return std::make_unique<WaveData>(wd);

}
void App::playSound(WaveData* waveData, float volume)
{
  SDL_BindAudioStream(audioDeviceId, waveData->stream);
  SDL_SetAudioStreamGain(waveData->stream, volume);
  currentlyPlayingSounds.push_back(waveData);
}

} // namespace invaderz

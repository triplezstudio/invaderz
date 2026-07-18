
#include "App.hh"
#include "SdlException.hh"

namespace invaderz {
namespace {
constexpr auto WINDOW_TITLE = "invaderz";
}

App::App(const int width, const int height, IAudioManagerPtr audioManager)
  : runtime::CoreObject("app")
  , m_audioManager(std::move(audioManager))
{
  if (m_audioManager == nullptr)
  {
    throw std::invalid_argument("Expected non null audio manager");
  }

  initializeSdl(width, height);
}

App::~App()
{
  m_audioManager.reset();
  SDL_CloseAudioDevice(m_audioDeviceId);
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}

auto App::pollEvents() -> FrameData
{
  bool quit{false};
  SDL_Event event{};
  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_EVENT_QUIT)
    {
      quit = true;
    }
  }

  m_state.update();

  return FrameData{
    .quit    = quit,
    .state   = m_state,
    .elapsed = m_timer.restart(),
  };
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

namespace {
constexpr std::uint8_t ALPHA_OPAQUE = 255;

auto colorToRgb(const Color color) -> std::array<std::uint8_t, 3>
{
  switch (color)
  {
    case Color::ORANGE:
      return std::array<std::uint8_t, 3>{168, 119, 50};
    case Color::TURQUOISE:
      return std::array<std::uint8_t, 3>{90, 222, 209};
    case Color::BURGUNDY:
      return std::array<std::uint8_t, 3>{102, 0, 51};
    default:
      throw std::runtime_error("Unsupported color");
  }
}
} // namespace

void App::renderRectangle(const Eigen::Vector3f &position,
                          const Eigen::Vector3f &dims,
                          const Color color)
{
  auto rgb = colorToRgb(color);

  SDL_SetRenderDrawColor(m_renderer, rgb[0], rgb[1], rgb[2], ALPHA_OPAQUE);
  SDL_FRect rect{
    .x = position(0),
    .y = position(1),
    .w = dims(0),
    .h = dims(1),
  };
  SDL_RenderFillRect(m_renderer, &rect);
}

void App::playOnce(const SoundId id, const float volume)
{
  auto &sound = m_audioManager->getSound(id);

  sound.bindToAudioDevice(m_audioDeviceId, volume);
  m_currentlyPlayingSounds.push_back(PlayingSound{.id = id, .mode = Mode::ONCE});
}

void App::update()
{
  for (const auto &sound : m_currentlyPlayingSounds)
  {
    updatePlayingSound(sound);
  }

  std::erase_if(m_currentlyPlayingSounds, [this](const PlayingSound &sound) {
    auto &soundData = m_audioManager->getSound(sound.id);
    return soundData.isFinished();
  });
}

void App::initializeSdl(const int width, const int height)
{
  if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_AUDIO))
  {
    throw runtime::SdlException("Failed to initialize SDL");
  }

  if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, width, height, 0, &m_window, &m_renderer))
  {
    throw runtime::SdlException("Failed to initialize window/renderer");
  }

  initializeAudio();
}

void App::initializeAudio()
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

void App::updatePlayingSound(const PlayingSound &sound)
{
  auto &soundData = m_audioManager->getSound(sound.id);

  if (!soundData.isFinished())
  {
    soundData.update();
  }
}

} // namespace invaderz

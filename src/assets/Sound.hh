
#pragma once

#include <SDL3/SDL.h>
#include <memory>
#include <string_view>

// This forward declaration allows to give access to the internals
// of the SoundData class to SdlAssetManager.
namespace invaderz {
class SdlAssetManager;
}

namespace invaderz::assets {

class SoundData
{
  public:
  SoundData(const std::string_view filePath);
  ~SoundData();

  private:
  SDL_AudioStream *m_stream{nullptr};
  uint8_t *m_data{nullptr};
  int m_lengthInBytes{0};

  friend class invaderz::SdlAssetManager;
};

using SoundDataPtr = std::unique_ptr<SoundData>;

} // namespace invaderz::assets

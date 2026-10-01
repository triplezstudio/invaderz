
#pragma once

#include "ISoundLoader.hh"
#include <memory>
#include <string_view>

namespace invaderz::assets {

class Sound
{
  public:
  Sound(const std::string_view filePath, ISoundLoader *loader);
  ~Sound();

  auto getRaw() const -> MIX_Audio *;

  private:
  MIX_Audio *m_stream{nullptr};
};

using SoundPtr = std::unique_ptr<Sound>;

} // namespace invaderz::assets

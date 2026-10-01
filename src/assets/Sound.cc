
#include "Sound.hh"
#include "SdlException.hh"

namespace invaderz::assets {

Sound::Sound(const std::string_view filePath, ISoundLoader *loader)
{
  m_stream = MIX_LoadAudio(loader, filePath.data(), true);
  if (m_stream == nullptr)
  {
    throw runtime::SdlException("Couldn't load file \"" + std::string(filePath) + "\"");
  }
}

Sound::~Sound()
{
  MIX_DestroyAudio(m_stream);
}

auto Sound::getRaw() const -> MIX_Audio *
{
  return m_stream;
}

} // namespace invaderz::assets

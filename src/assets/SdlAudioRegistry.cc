
#include "SdlAudioRegistry.hh"

namespace invaderz {

SdlAudioRegistry::SdlAudioRegistry(ISoundLoader *loader)
  : runtime::CoreObject("audio")
  , m_loader(std::move(loader))
{
  addModule("registry");

  if (m_loader == nullptr)
  {
    throw std::invalid_argument("Expected non null sound loader");
  }
}

auto SdlAudioRegistry::registerSound(const std::string_view filePath) -> SoundId
{
  auto data     = std::make_unique<assets::Sound>(filePath, m_loader);
  const auto id = m_nextId.fetch_add(1);

  m_sounds.emplace(id, std::move(data));

  return id;
}

void SdlAudioRegistry::unregister(const SoundId &sound)
{
  m_sounds.erase(sound);
}

auto SdlAudioRegistry::getSound(const SoundId id) const -> assets::Sound &
{
  const auto maybeSound = m_sounds.find(id);
  if (maybeSound == m_sounds.end())
  {
    error("Unknown sound " + std::to_string(id));
  }

  return *maybeSound->second;
}

} // namespace invaderz

#include "SdlTextureRegistry.hh"

namespace invaderz {

SdlTextureRegistry::SdlTextureRegistry(ITextureLoader *loader)
  : runtime::CoreObject("texture")
  , m_loader(std::move(loader))
{
  addModule("registry");

  if (m_loader == nullptr)
  {
    throw std::invalid_argument("Expected non null texture loader");
  }
}

auto SdlTextureRegistry::registerTexture(const std::string_view filePath) -> TextureId
{
  auto data     = std::make_unique<assets::Texture>(filePath, m_loader);
  const auto id = m_nextId.fetch_add(1);

  m_textures.emplace(id, std::move(data));

  return id;
}

void SdlTextureRegistry::unregister(const TextureId &texture)
{
  m_textures.erase(texture);
}

auto SdlTextureRegistry::getTexture(const TextureId id) const -> assets::Texture &
{
  const auto maybeTexture = m_textures.find(id);
  if (maybeTexture == m_textures.end())
  {
    error("Unknown texture " + std::to_string(id));
  }

  return *maybeTexture->second;
}

} // namespace invaderz

#include "SdlAssetManager.hh"
#include "SdlException.hh"
#include <SDL3/SDL.h>

namespace invaderz {

SdlAssetManager::SdlAssetManager()
  : runtime::CoreObject("manager")
{
  addModule("assets");
}

SdlAssetManager::~SdlAssetManager()
{
  m_sounds.clear();
  m_textures.clear();
}

auto SdlAssetManager::registerSound(const std::string_view filePath) -> assets::Asset
{
  auto data = std::make_unique<assets::Sound>(filePath);

  auto assetId = m_nextAssetId.fetch_add(1);
  auto asset   = assets::Asset{.type = assets::Type::SOUND, .id = assetId};

  m_sounds.emplace(assetId, std::move(data));

  return asset;
}

auto SdlAssetManager::getSound(const SoundId id) const -> assets::Sound &
{
  const auto maybeSound = m_sounds.find(id);
  if (maybeSound == m_sounds.end())
  {
    error("Unknown sound " + std::to_string(id));
  }

  return *maybeSound->second;
}

auto SdlAssetManager::registerTexture(const std::string_view filePath) -> assets::Asset
{
  auto data = std::make_unique<assets::Texture>(filePath);

  auto assetId = m_nextAssetId.fetch_add(1);
  auto asset   = assets::Asset{.type = assets::Type::TEXTURE, .id = assetId};

  m_textures.emplace(assetId, std::move(data));

  return asset;
}

auto SdlAssetManager::getTexture(const TextureId id) const -> assets::Texture &
{
  const auto maybeTexture = m_textures.find(id);
  if (maybeTexture == m_textures.end())
  {
    error("Unknown texture " + std::to_string(id));
  }

  return *maybeTexture->second;
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
    case assets::Type::TEXTURE:
      m_textures.erase(asset.id);
      break;
    default:
      error("Unsupported asset type " + str(asset.type));
  }
}

} // namespace invaderz
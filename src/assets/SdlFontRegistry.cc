
#include "SdlFontRegistry.hh"

namespace invaderz {

SdlFontRegistry::SdlFontRegistry(IFontLoader *loader)
  : runtime::CoreObject("font")
  , m_loader(std::move(loader))
{
  addModule("registry");

  if (m_loader == nullptr)
  {
    throw std::invalid_argument("Expected non null foat loader");
  }
}

auto SdlFontRegistry::registerFont(const std::string_view filePath, const int pointSize) -> FontId
{
  auto data     = std::make_unique<assets::Font>(filePath, pointSize, m_loader);
  const auto id = m_nextId.fetch_add(1);

  m_fonts.emplace(id, std::move(data));

  return id;
}

void SdlFontRegistry::unregister(const FontId &Font)
{
  m_fonts.erase(Font);
}

auto SdlFontRegistry::getFont(const FontId id) const -> assets::Font &
{
  const auto maybeFont = m_fonts.find(id);
  if (maybeFont == m_fonts.end())
  {
    error("Unknown font " + std::to_string(id));
  }

  return *maybeFont->second;
}

} // namespace invaderz
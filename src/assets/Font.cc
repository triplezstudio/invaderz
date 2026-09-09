
#include "Font.hh"
#include "SdlException.hh"

namespace invaderz::assets {

Font::Font(const std::string_view filePath, const int pointSize, IFontLoader *loader)
  : m_loader(std::move(loader))
{
  if (m_loader == nullptr)
  {
    throw std::invalid_argument("Expected non null font loader");
  }

  m_font = TTF_OpenFont(filePath.data(), pointSize);
  if (!m_font)
  {
    throw runtime::SdlException("Failed to create font for \"" + std::string(filePath) + "\"");
  }
}

Font::~Font()
{
  TTF_CloseFont(m_font);
}

auto Font::renderText(const std::string &text) -> TTF_Text *
{
  const auto maybeText = m_cache.find(text);
  if (maybeText != m_cache.end())
  {
    return maybeText->second;
  }

  auto ttfText  = TTF_CreateText(m_loader, m_font, text.c_str(), text.size());
  m_cache[text] = ttfText;

  return ttfText;
}

} // namespace invaderz::assets

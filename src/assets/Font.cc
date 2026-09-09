
#include "Font.hh"
#include "SdlException.hh"

namespace invaderz::assets {

Font::Font(const std::string_view filePath, const int pointSize)
{
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

} // namespace invaderz::assets

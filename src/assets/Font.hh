
#pragma once

#include "IFontLoader.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <string_view>

namespace invaderz::assets {

class Font
{
  public:
  Font(const std::string_view filePath, const int pointSize, IFontLoader *loader);
  ~Font();

  auto getTextSize(const std::string &text) const -> Eigen::Vector2i;

  auto renderText(const std::string &text) -> TTF_Text *;

  private:
  TTF_Font *m_font{nullptr};
  IFontLoader *m_loader{nullptr};

  std::unordered_map<std::string, TTF_Text *> m_cache{};
};

using FontPtr = std::unique_ptr<Font>;

} // namespace invaderz::assets

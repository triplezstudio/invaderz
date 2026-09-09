
#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <string_view>

namespace invaderz::assets {

class Font
{
  public:
  Font(const std::string_view filePath, const int pointSize);
  ~Font();

  auto getTextSize(const std::string &text) const -> Eigen::Vector2i;

  private:
  TTF_Font *m_font{nullptr};
};

using FontPtr = std::unique_ptr<Font>;

} // namespace invaderz::assets

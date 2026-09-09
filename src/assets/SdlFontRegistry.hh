
#pragma once

#include "CoreObject.hh"
#include "Font.hh"
#include <atomic>
#include <memory>
#include <string_view>
#include <unordered_map>

namespace invaderz {

using FontId = int;

class SdlFontRegistry : public runtime::CoreObject
{
  public:
  SdlFontRegistry();
  virtual ~SdlFontRegistry() = default;

  auto registerFont(const std::string_view filePath, const int pointSize) -> FontId;
  void unregister(const FontId &font);

  auto getFont(const FontId id) const -> assets::Font &;

  private:
  std::atomic<FontId> m_nextId{0};
  std::unordered_map<FontId, assets::FontPtr> m_fonts{};
};

using SdlFontRegistryPtr = std::unique_ptr<SdlFontRegistry>;

} // namespace invaderz

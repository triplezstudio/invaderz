
#pragma once

#include <string>

namespace invaderz::assets {

enum class Type
{
  SOUND,
  TEXTURE,
  UNKNOWN
};

using Id = int;

struct Asset
{
  Type type{};
  Id id{};

  bool valid() const;
};

auto str(const Type &type) -> std::string;

} // namespace invaderz::assets

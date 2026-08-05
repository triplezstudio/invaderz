
#include "Asset.hh"

namespace invaderz::assets {

bool Asset::valid() const
{
  return type != Type::UNKNOWN;
}

auto str(const Type &type) -> std::string
{
  switch (type)
  {
    case Type::SOUND:
      return "sound";
    case Type::TEXTURE:
      return "texture";
    case Type::UNKNOWN:
      return "unknown";
    default:
      return "unsupported";
  }
}

} // namespace invaderz::assets
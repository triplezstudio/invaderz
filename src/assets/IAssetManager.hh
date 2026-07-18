
#pragma once

#include "Asset.hh"

namespace invaderz {

class IAssetManager
{
  public:
  IAssetManager()          = default;
  virtual ~IAssetManager() = default;

  virtual void unregister(const assets::Asset &asset) = 0;
};

} // namespace invaderz

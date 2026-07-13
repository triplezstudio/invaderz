
#pragma once

#include "FrameData.hh"
#include "World.hh"
#include <memory>

namespace invaderz {

class PlayerUpdater
{
  public:
  PlayerUpdater(World &world);
  ~PlayerUpdater() = default;

  void update(const FrameData &data);

  private:
  World &m_world;
};

using PlayerUpdaterPtr = std::unique_ptr<PlayerUpdater>;

} // namespace invaderz

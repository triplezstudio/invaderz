
#pragma once

#include "FrameData.hh"
#include "ILevel.hh"
#include <memory>

namespace invaderz {

class PlayerUpdater
{
  public:
  PlayerUpdater(ILevel &level);
  ~PlayerUpdater() = default;

  void update(const FrameData &data, Effects &effects);

  private:
  ILevel &m_level;
};

using PlayerUpdaterPtr = std::unique_ptr<PlayerUpdater>;

} // namespace invaderz

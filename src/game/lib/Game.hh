
#pragma once

#include "CoreObject.hh"
#include "EventData.hh"
#include "IRenderer.hh"
#include <eigen3/Eigen/Eigen>

namespace invaderz {

class Game : public runtime::CoreObject
{
  public:
  Game();
  ~Game() override = default;

  bool update(const EventData &data);
  void render(IRenderer &renderer);

  private:
  Eigen::Vector3f m_playerPosition{Eigen::Vector3f::Zero()};
};

} // namespace invaderz

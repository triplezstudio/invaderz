
#include "PlayerUpdater.hh"

namespace invaderz {

PlayerUpdater::PlayerUpdater(World &world)
  : m_world(world)
{}

namespace {
auto collectMotion(const FrameData &data) -> Eigen::Vector3f
{
  Eigen::Vector3f motion = Eigen::Vector3f::Zero();

  if (data.state.held(keyboard::LEFT))
  {
    motion(0) -= 1.0f;
  }
  if (data.state.held(keyboard::RIGHT))
  {
    motion(0) += 1.0f;
  }

  motion.normalize();
  motion *= data.elapsed;

  return motion;
}
} // namespace

void PlayerUpdater::update(const FrameData &data)
{
  const auto motion = collectMotion(data);
  m_world.movePlayer(motion);

  if (data.state.released(keyboard::SPACE))
  {
    m_world.fire();
  }
}

} // namespace invaderz

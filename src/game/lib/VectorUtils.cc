
#include "VectorUtils.hh"
#include <format>

namespace invaderz {

auto str(const Eigen::Vector3f &v) -> std::string
{
  return std::format("{}x{}x{}", v(0), v(1), v(2));
}

} // namespace invaderz

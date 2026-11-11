
#include "FrameData.hh"
#include <cmath>

namespace invaderz {
namespace {
constexpr auto MICROSECONDS_IN_SECONDS = 1'000'000;
constexpr auto ROUNDING                = 100;
} // namespace

auto FrameData::frameTimeMicroseconds() const -> int
{
  const auto microseconds = static_cast<int>(std::floor(elapsed * MICROSECONDS_IN_SECONDS));
  return (microseconds / ROUNDING) * ROUNDING;
}

} // namespace invaderz

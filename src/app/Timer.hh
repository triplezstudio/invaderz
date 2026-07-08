
#pragma once

#include <SDL3/SDL.h>
#include <cstdint>

namespace invaderz {

class Timer
{
  public:
  void start();

  /// @brief - Restart the time measurment interval and sets the starting point
  /// to the current time.
  /// @return - the seconds elapsed since the last call to start
  auto restart() -> float;

  private:
  std::uint64_t m_start{0};
  float m_frequency{static_cast<float>(SDL_GetPerformanceFrequency())};
};

} // namespace invaderz



#include "Timer.hh"

#include <iostream>

namespace invaderz {

void Timer::start()
{
  m_start = SDL_GetPerformanceCounter();
}

auto Timer::restart() -> float
{
  const auto now     = SDL_GetPerformanceCounter();
  const auto elapsed = now - m_start;

  m_start = now;

  return elapsed / m_frequency;
}

} // namespace invaderz


#include "SdlException.hh"
#include <SDL3/SDL.h>

namespace invaderz::runtime {

constexpr auto MODULE_NAME = "sdl";

SdlException::SdlException(const std::string &message)
  : CoreException(MODULE_NAME, message, SDL_GetError())
{}

} // namespace invaderz::runtime

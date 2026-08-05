
#pragma once

#include "Renderer.hh"
#include <memory>

namespace invaderz {

using IRenderer    = Renderer;
using IRendererPtr = std::unique_ptr<Renderer>;

} // namespace invaderz

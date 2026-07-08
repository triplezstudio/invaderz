
#pragma once

#include "Keyboard.hh"
#include "Mouse.hh"
#include <SDL3/SDL.h>
#include <vector>

namespace invaderz {

class InputState
{
  public:
  InputState();

  void update();

  bool pressed(const mouse::Button button) const;
  bool held(const mouse::Button button) const;
  bool released(const mouse::Button button) const;

  bool pressed(const keyboard::Keys key) const;
  bool held(const keyboard::Keys key) const;
  bool released(const keyboard::Keys key) const;

  private:
  float m_mPosX{0.0f};
  float m_mPosY{0.0f};

  std::vector<keyboard::State> m_keys{};
  std::vector<mouse::State> m_buttons{};

  bool m_shift{false};

  void updateKeyboardState();
  void updateMouseState();
};

} // namespace invaderz

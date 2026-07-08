
#include "InputState.hh"

#include <iostream>

namespace invaderz {

InputState::InputState()
  : m_keys(keyboard::KEYS_COUNT, keyboard::FREE)
  , m_buttons(mouse::BUTTONS_COUNT, mouse::FREE)
{}

void InputState::update()
{
  updateKeyboardState();
  updateMouseState();
}

bool InputState::pressed(const mouse::Button button) const
{
  return mouse::PRESSED == m_buttons[button];
}

bool InputState::held(const mouse::Button button) const
{
  return mouse::HELD == m_buttons[button];
}

bool InputState::released(const mouse::Button button) const
{
  return mouse::RELEASED == m_buttons[button];
}

bool InputState::pressed(const keyboard::Keys key) const
{
  return keyboard::PRESSED == m_keys[key];
}

bool InputState::held(const keyboard::Keys key) const
{
  return keyboard::PRESSED == m_keys[key] || keyboard::HELD == m_keys[key];
}

bool InputState::released(const keyboard::Keys key) const
{
  return keyboard::RELEASED == m_keys[key];
}

namespace {
auto mapSdlKey(const SDL_Scancode key) -> keyboard::Keys
{
  switch (key)
  {
    case SDL_SCANCODE_A:
      return keyboard::A;
    case SDL_SCANCODE_B:
      return keyboard::B;
    case SDL_SCANCODE_C:
      return keyboard::C;
    case SDL_SCANCODE_D:
      return keyboard::D;
    case SDL_SCANCODE_E:
      return keyboard::E;
    case SDL_SCANCODE_F:
      return keyboard::F;
    case SDL_SCANCODE_G:
      return keyboard::G;
    case SDL_SCANCODE_H:
      return keyboard::H;
    case SDL_SCANCODE_I:
      return keyboard::I;
    case SDL_SCANCODE_J:
      return keyboard::J;
    case SDL_SCANCODE_K:
      return keyboard::K;
    case SDL_SCANCODE_L:
      return keyboard::L;
    case SDL_SCANCODE_M:
      return keyboard::M;
    case SDL_SCANCODE_N:
      return keyboard::N;
    case SDL_SCANCODE_O:
      return keyboard::O;
    case SDL_SCANCODE_P:
      return keyboard::P;
    case SDL_SCANCODE_Q:
      return keyboard::Q;
    case SDL_SCANCODE_R:
      return keyboard::R;
    case SDL_SCANCODE_S:
      return keyboard::S;
    case SDL_SCANCODE_T:
      return keyboard::T;
    case SDL_SCANCODE_U:
      return keyboard::U;
    case SDL_SCANCODE_V:
      return keyboard::V;
    case SDL_SCANCODE_W:
      return keyboard::W;
    case SDL_SCANCODE_X:
      return keyboard::X;
    case SDL_SCANCODE_Y:
      return keyboard::Y;
    case SDL_SCANCODE_Z:
      return keyboard::Z;
    case SDL_SCANCODE_1:
      return keyboard::K1;
    case SDL_SCANCODE_2:
      return keyboard::K2;
    case SDL_SCANCODE_3:
      return keyboard::K3;
    case SDL_SCANCODE_4:
      return keyboard::K4;
    case SDL_SCANCODE_5:
      return keyboard::K5;
    case SDL_SCANCODE_6:
      return keyboard::K6;
    case SDL_SCANCODE_7:
      return keyboard::K7;
    case SDL_SCANCODE_8:
      return keyboard::K8;
    case SDL_SCANCODE_9:
      return keyboard::K9;
    case SDL_SCANCODE_0:
      return keyboard::K0;
    case SDL_SCANCODE_RETURN:
      return keyboard::RETURN;
    case SDL_SCANCODE_ESCAPE:
      return keyboard::ESCAPE;
    case SDL_SCANCODE_BACKSPACE:
      return keyboard::BACK;
    case SDL_SCANCODE_TAB:
      return keyboard::TAB;
    case SDL_SCANCODE_SPACE:
      return keyboard::SPACE;
    case SDL_SCANCODE_RIGHT:
      return keyboard::RIGHT;
    case SDL_SCANCODE_LEFT:
      return keyboard::LEFT;
    case SDL_SCANCODE_DOWN:
      return keyboard::DOWN;
    case SDL_SCANCODE_UP:
      return keyboard::UP;
    default:
      return keyboard::NONE;
  }
}
} // namespace

void InputState::updateKeyboardState()
{
  int size{0};
  auto keys = SDL_GetKeyboardState(&size);

  for (auto id = 0; id < size; ++id)
  {
    auto key = mapSdlKey(static_cast<SDL_Scancode>(id));

    if (keys[id])
    {
      if (m_keys[key] == keyboard::PRESSED)
      {
        m_keys[key] = keyboard::HELD;
      }
      else if (m_keys[key] != keyboard::HELD)
      {
        m_keys[key] = keyboard::PRESSED;
      }
    }
    else
    {
      if (m_keys[key] == keyboard::RELEASED)
      {
        m_keys[key] = keyboard::FREE;
      }
      else if (m_keys[key] != keyboard::FREE)
      {
        m_keys[key] = keyboard::RELEASED;
      }
    }
  }
}

void InputState::updateMouseState()
{
  auto buttons = SDL_GetMouseState(&m_mPosX, &m_mPosY);

  auto updateButtonState =
    [](std::vector<mouse::State> &buttons, const mouse::Button button, const bool pressed) {
      if (pressed)
      {
        if (buttons[button] == mouse::PRESSED)
        {
          buttons[button] = mouse::HELD;
        }
        else if (buttons[button] != mouse::HELD)
        {
          buttons[button] = mouse::PRESSED;
        }
      }
      else
      {
        if (buttons[button] == mouse::RELEASED)
        {
          buttons[button] = mouse::FREE;
        }
        else if (buttons[button] != mouse::FREE)
        {
          buttons[button] = mouse::RELEASED;
        }
      }
    };

  updateButtonState(m_buttons, mouse::LEFT, buttons & SDL_BUTTON_LEFT);
  updateButtonState(m_buttons, mouse::RIGHT, buttons & SDL_BUTTON_RIGHT);
  updateButtonState(m_buttons, mouse::MIDDLE, buttons & SDL_BUTTON_MIDDLE);
}

} // namespace invaderz

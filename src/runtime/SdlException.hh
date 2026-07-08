

#pragma once

#include "CoreException.hh"

namespace invaderz::runtime {

class SdlException : public CoreException
{
  public:
  SdlException(const std::string &message);
  ~SdlException() noexcept override = default;

  private:
  std::string m_message{};
};

} // namespace invaderz::runtime

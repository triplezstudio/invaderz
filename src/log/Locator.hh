
#pragma once

#include "ILogger.hh"
#include <string>

namespace invaderz::log {

class Locator
{
  public:
  static void initialize();

  static auto getLogger() -> ILogger &;

  static void provide(ILogger *logger);

  private:
  static ILogger *m_logger;
};

} // namespace invaderz::log

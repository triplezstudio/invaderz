
#include "CoreException.hh"
#include "Locator.hh"
#ifdef _WINDOWS
#include <windows.h>
#include <dbghelp.h>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#else
#include <execinfo.h>
#endif
#include <vector>

namespace invaderz::runtime {
namespace {
constexpr auto MAX_STACK_TRACE_DEPTH = 32u;

auto getTraceSize(std::vector<void *> & addresses) -> size_t
{
#ifdef _WINDOWS
  return CaptureStackBackTrace(
    0,
    MAX_STACK_TRACE_DEPTH,
    addresses.data(),
    nullptr
  );
#else
  return backtrace(addresses.data(), addresses.size());
#endif
}


auto resolveStackTraceSymbols(std::vector<void*>& addresses) -> std::vector<std::string>
{

#ifdef _WINDOWS
  HANDLE process = GetCurrentProcess();

  SymInitialize(process, nullptr, TRUE);

  std::vector<std::string> result;
  result.reserve(addresses.size());

  constexpr size_t MAX_SYMBOL_NAME_LENGTH = 256;

  auto* symbol = reinterpret_cast<SYMBOL_INFO*>(
    std::calloc(
      1,
      sizeof(SYMBOL_INFO) + MAX_SYMBOL_NAME_LENGTH * sizeof(char)
        )
  );

  symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
  symbol->MaxNameLen = MAX_SYMBOL_NAME_LENGTH - 1;

  for (void* addressPtr : addresses)
  {
    DWORD64 address = reinterpret_cast<DWORD64>(addressPtr);

    DWORD64 displacement = 0;

    if (SymFromAddr(process, address, &displacement, symbol))
    {
      std::ostringstream line;
      line << symbol->Name
           << " + 0x" << std::hex << displacement;

      result.push_back(line.str());
    }
    else
    {
      std::ostringstream line;
      line << "<unknown> at 0x" << std::hex << address;
      result.push_back(line.str());
    }
  }

  std::free(symbol);
  SymCleanup(process);
  return result;

#elif defined __linux__

    return backtrace_symbols(addresses.data(), addresses.size());

#endif
}

auto retrieveStackTrace() -> std::string
{
  std::vector<void *> addresses(MAX_STACK_TRACE_DEPTH);
  const auto size = getTraceSize(addresses);
  addresses.resize(size);

  const auto funcs = resolveStackTraceSymbols(addresses);

  std::string stackTrace{};
  for (auto i = 0u; i < addresses.size(); ++i)
  {
    stackTrace += " at ";
    stackTrace += funcs[i];
    stackTrace += "\n";
  }

#ifdef __linux__
  // https://man7.org/linux/man-pages/man3/backtrace.3.html
  free(funcs);
#endif

  return stackTrace;
}
} // namespace

CoreException::CoreException(const std::string_view module,
                             const std::string &message,
                             const std::optional<std::string> &cause) noexcept
  : std::exception()
  , m_message(message)
{
  log::Locator::getLogger().error(module, message, cause);
  log::Locator::getLogger().error(module, retrieveStackTrace());
}

CoreException::CoreException(const std::string_view module,
                             const std::string &message,
                             const CoreException &cause) noexcept
  : std::exception()
  , m_message(message)
{
  log::Locator::getLogger().error(module, message, cause.what());
  log::Locator::getLogger().error(module, retrieveStackTrace());
}

auto CoreException::what() const throw() -> const char *
{
  return m_message.c_str();
}

} // namespace invaderz::runtime

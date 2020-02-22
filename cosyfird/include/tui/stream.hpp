#pragma once

#include "tui/window.hpp"

#include <string>
#include <chrono>

namespace csf
{
namespace tui
{

class Window;

/// @brief Prints a stream to a specific Window
class Stream
{
  public:
    Stream(Window& window);
    virtual ~Stream();
    Stream(const Stream&) = default;
    Stream(Stream&&) = default;
    Stream& operator=(const Stream&) = default;
    Stream& operator=(Stream&&) = default;

    Stream operator<<(const char* value);

    template <typename T>
    Stream operator<<(const T& value);

  private:
    Window& m_window;

    using SysClock = std::chrono::system_clock;
    using TimePoint = std::chrono::time_point<SysClock>;

    TimePoint m_timepoint;
    std::string m_message;
};
} // namespace tui
} // namespace csf

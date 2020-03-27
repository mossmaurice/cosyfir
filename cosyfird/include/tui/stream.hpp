#pragma once

#include "tui/window.hpp"

#include <chrono>
#include <string>

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
    virtual ~Stream() = default;
    Stream(const Stream&) = default;
    Stream(Stream&&) = default;
    Stream& operator=(const Stream&) = default;
    Stream& operator=(Stream&&) = default;

    Stream& operator<<(const char* value);
    Stream& operator<<(const std::string& string);
    template <typename T>
    Stream& operator<<(const T& value);

  protected:
    Window& m_window;

    using SysClock = std::chrono::system_clock;
    using TimePoint = std::chrono::time_point<SysClock>;

    TimePoint m_timePoint{SysClock::now()};
    std::string m_message;
};

class MessageStream : public Stream
{
  public:
    MessageStream(Window& window);
    ~MessageStream() override;
};

class StatusStream : public Stream
{
  public:
    StatusStream(Window& window);
    ~StatusStream() override;
};

} // namespace tui
} // namespace csf

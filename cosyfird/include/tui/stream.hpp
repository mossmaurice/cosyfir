#pragma once

#include "tui/window.hpp"

#include <chrono>
#include <iomanip>
#include <ios>
#include <sstream>
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

    /// @todo not for floats, std::bitset?
    template <typename T>
    Stream& operator<<(const T& value)
    {
        std::stringstream sstream;
        // ANDing the range is needed due to the fact that chars are
        // interpreted as characters and not uint8_t
        const auto max = std::numeric_limits<T>::max();
        const auto min = std::numeric_limits<T>::min();
        const auto range = max - min;
        sstream << std::uppercase << std::internal << std::setfill('0')
                << std::setw(2) << std::hex << (range & value);
        m_message.append(sstream.str());
        return *this;
    }

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

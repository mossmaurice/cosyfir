#include "tui/stream.hpp"

#include <iomanip>

namespace csf
{
namespace tui
{

Stream::Stream(Window& window)
    : m_window(window)
{
}

Stream& Stream::operator<<(const char* charString)
{
    m_message.append(charString);
    return *this;
}

Stream& Stream::operator<<(const std::string& string)
{
    m_message.append(string);
    return *this;
}

template <typename T>
Stream& Stream::operator<<(const T& value)
{
    /// @todo std::bitset
    m_message.append(std::to_string(value));
    return *this;
}

MessageStream::MessageStream(Window& window)
    : Stream(window)
{
}

MessageStream::~MessageStream()
{
    m_window.addstr(1, 2, m_message.c_str());

    // Reprint border
    m_window.printBorder();
}

StatusStream::StatusStream(Window& window)
    : Stream(window)
{
}

StatusStream::~StatusStream()
{
    // Get current position of the cursor
    int x;
    int y;
    m_window.getyx(x, y);

    m_message.append("\n");

    // Add date and time to output
    std::stringstream strMessage;
    std::time_t time = std::chrono::system_clock::to_time_t(m_timePoint);
    strMessage << "[" << std::put_time(std::localtime(&time), "%F %T") << "]"
               << " " << m_message;
    std::string messageWithDate = strMessage.str();

    // Print message to window
    m_window.addstr(x, 2, messageWithDate.c_str());
    m_window.move(x + 1, 2);

    // Reprint border
    m_window.printBorder();

    // In case we reached the bottom, let the oldest line disappear
    if (x > m_window.lines())
    {
        m_window.scroll();
    }
}

} // namespace tui
} // namespace csf

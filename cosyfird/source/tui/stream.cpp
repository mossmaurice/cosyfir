#include "tui/stream.hpp"

#include <iomanip>

namespace csf
{
namespace tui
{

Stream::Stream(Window& window) : m_window(window) {}

Stream::~Stream()
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

Stream& Stream::operator<<(const char* value)
{
    m_message.append(value);
    return *this;
}

template <typename T>
Stream& Stream::operator<<(const T& value)
{
    /// @todo std::bitset
    m_message.append(std::to_string(value));
    return *this;
}

} // namespace tui
} // namespace csf

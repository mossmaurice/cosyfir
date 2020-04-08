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

Stream& Stream::operator<<(const std::stringstream& sstring)
{
    m_message.append(sstring.str());
    return *this;
}

MessageStream::MessageStream(Window& window)
    : Stream(window)
{
}

MessageStream::~MessageStream()
{
    m_window.showString(m_message);
}

StatusStream::StatusStream(Window& window)
    : Stream(window)
{
}

StatusStream::~StatusStream()
{
    m_message.append("\n");

    // Add date and time to output
    std::stringstream strMessage;
    std::time_t time = std::chrono::system_clock::to_time_t(m_timePoint);
    strMessage << "[" << std::put_time(std::localtime(&time), "%F %T") << "]"
               << " " << m_message;
    std::string messageWithDate = strMessage.str();

    m_window.addString(messageWithDate);
}

} // namespace tui
} // namespace csf

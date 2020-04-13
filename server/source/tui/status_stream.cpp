#include "tui/status_stream.hpp"

#include <iomanip>

namespace csf
{
namespace tui
{

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

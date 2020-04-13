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

} // namespace tui
} // namespace csf

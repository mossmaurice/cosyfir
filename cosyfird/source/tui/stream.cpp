#include "tui/stream.hpp"

namespace csf
{
namespace tui
{

Stream::Stream(Window& window) : m_window(window) {}

Stream::~Stream()
{
    m_message.append("\n");
    m_window.printw(m_message.c_str());
    m_window.refresh();
}

Stream Stream::operator<<(const char* value)
{
    m_message.append(value);
    return *this;
}

template <typename T>
Stream Stream::operator<<(const T& value)
{
    /// @todo std::bitset
    m_message.append(std::to_string(value));
    return *this;
}

} // namespace tui
} // namespace csf

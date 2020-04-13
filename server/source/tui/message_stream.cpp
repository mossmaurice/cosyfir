#include "tui/message_stream.hpp"

#include <iomanip>

namespace csf
{
namespace tui
{

MessageStream::MessageStream(Window& window)
    : Stream(window)
{
}

MessageStream::~MessageStream()
{
    m_window.showString(m_message);
}

} // namespace tui
} // namespace csf

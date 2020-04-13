#pragma once

#include "tui/stream.hpp"

namespace csf
{
namespace tui
{

class Window;

class MessageStream : public Stream
{
  public:
    MessageStream(Window& window);
    ~MessageStream() override;
};

} // namespace tui
} // namespace csf

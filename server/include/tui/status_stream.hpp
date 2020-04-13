#pragma once

#include "tui/stream.hpp"

namespace csf
{
namespace tui
{

class Window;

class StatusStream : public Stream
{
  public:
    StatusStream(Window& window);
    ~StatusStream() override;
};

} // namespace tui
} // namespace csf

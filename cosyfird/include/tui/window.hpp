#pragma once

#include "cursesw.h"

namespace csf
{
namespace tui
{
class Window : public NCursesWindow
{
  public:
    Window();
    virtual ~Window() = default;
    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;
};
} // namespace tui
} // namespace csf

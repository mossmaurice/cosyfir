#pragma once

#include "tui/stream.hpp"

#include "cursesw.h"

namespace csf
{
namespace tui
{

class Stream;

class Window : public NCursesWindow
{
  public:
    Window(int lines, int cols, int beginX, int beginY);
    virtual ~Window() = default;
    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

    Stream stream();
};
} // namespace tui
} // namespace csf

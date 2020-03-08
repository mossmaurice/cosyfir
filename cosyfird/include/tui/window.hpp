#pragma once

#include "tui/stream.hpp"

#include "cursesw.h"

namespace csf
{
namespace tui
{

class Stream;

class Window : public NCursesColorWindow
{
  public:
    Window(std::string title, int lines, int cols, int beginX, int beginY);
    virtual ~Window() = default;
    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

    Stream print();
    /// @todo
    // HexStream displayHex();
    // Stream showMessage();

    void printBorder();

  private:
    std::string m_title;
};
} // namespace tui
} // namespace csf

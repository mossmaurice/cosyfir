#pragma once

#include "tui/stream.hpp"

#include "cursesw.h"

namespace csf
{
namespace tui
{

class StatusStream;
class MessageStream;

class Window : public NCursesColorWindow
{
  public:
    Window(std::string title, int lines, int cols, int beginX, int beginY);
    virtual ~Window() = default;
    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

    StatusStream printLine();

    MessageStream display();

    void printBorder();

  private:
    std::string m_title;
};
} // namespace tui
} // namespace csf

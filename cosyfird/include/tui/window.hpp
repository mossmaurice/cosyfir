#pragma once

#include "tui/stream.hpp"

#include "cursesw.h"

namespace csf
{
namespace tui
{

class StatusStream;
class MessageStream;

class Window
{
  public:
    Window(std::string title, int lines, int cols, int beginX, int beginY);
    virtual ~Window() = default;
    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

    /// @brief Prints a line with date
    StatusStream printLine();

    /// @brief Displays stream, replacing old content
    MessageStream display();

    /// @brief Prints string in new line
    void addStringWithNewline(std::string& string);

    void showString(std::string& string);

    void printTitle();

    /// @brief Returns true if 'q' key was pressed
    bool waitForExit();

  private:
    std::string m_title;
    NCursesWindow m_parentWindow;
    NCursesWindow m_subWindow;
};
} // namespace tui
} // namespace csf

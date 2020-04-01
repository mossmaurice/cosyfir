#pragma once

#include "tui/stream.hpp"
#include "tui/tuiapp.hpp"

#include "cursesw.h"

namespace csf
{
namespace tui
{

class StatusStream;
class MessageStream;

enum class TextPosition
{
    LEFT,
    CENTER
};

class Window
{
  public:
    Window(const std::string title,
           const uint16_t lines,
           const uint16_t cols,
           const uint16_t beginX,
           const uint16_t beginY,
           const TextPosition = TextPosition::LEFT,
           const ColorPair colorPair = ColorPair::BLACK_ON_WHITE);
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

    /// @brief Returns true if 'q' key was pressed
    bool waitForExit();

  private:
    NCursesWindow m_parentWindow;
    NCursesWindow m_subWindow;
    std::string m_title;
    TextPosition m_textPosition;
};
} // namespace tui
} // namespace csf

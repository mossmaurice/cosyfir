#include "tui/window.hpp"
namespace csf
{
namespace tui
{
Window::Window(std::string title, int lines, int cols, int beginX, int beginY)
    : m_parentWindow(lines, cols, beginX, beginY)
    , m_subWindow(m_parentWindow, true)
    , m_title(title)
{
    m_parentWindow.useColors();
    m_subWindow.useColors();

    // Refresh automatically
    m_subWindow.immedok(true);
    m_parentWindow.immedok(true);

    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    m_subWindow.bkgd(COLOR_PAIR(1));
    m_parentWindow.bkgd(COLOR_PAIR(1));

    printTitle();

    m_subWindow.scrollok(true);
}

/// @todo StatusStream& ?
StatusStream Window::printLine()
{
    return StatusStream(*this);
}

MessageStream Window::display()
{
    return MessageStream(*this);
}

void Window::addStringWithNewline(std::string& string)
{
    // Get current position of the cursor
    int x;
    int y;
    m_subWindow.getyx(x, y);

    // Print message to window
    m_subWindow.addstr(x, 0, string.c_str());
    m_subWindow.move(x + 1, 0);

    // In case we reached the bottom, let the oldest line disappear
    if (x > m_subWindow.lines())
    {
        m_subWindow.scroll();
    }
};

void Window::showString(std::string& string)
{
    m_subWindow.clear();
    /// @todo add possibility for centered output
    m_subWindow.addstr(0, 0, string.c_str());
}

bool Window::waitForExit()
{
    return m_subWindow.getch() != 'q';
}

void Window::printTitle()
{
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    m_parentWindow.color_set(2);
    m_parentWindow.addstr(0, 3, m_title.c_str());
    m_parentWindow.color_set(1);
}

} // namespace tui
} // namespace csf

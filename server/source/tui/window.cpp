#include "tui/window.hpp"
namespace csf
{
namespace tui
{
Window::Window(const std::string title,
               const uint16_t lines,
               const uint16_t cols,
               const uint16_t beginX,
               const uint16_t beginY,
               const TextPosition textPosition,
               const ColorPair colorPair)
    : m_parentWindow(lines, cols, beginX, beginY)
    , m_subWindow(m_parentWindow, true)
    , m_title(title)
    , m_textPosition(textPosition)
{
    using ColorPairType = std::underlying_type<ColorPair>::type;

    // Refresh automatically
    m_subWindow.immedok(true);
    m_parentWindow.immedok(true);

    // Allow scrolling
    m_subWindow.scrollok(true);

    // Set background colors
    m_subWindow.bkgd(
        COLOR_PAIR(static_cast<ColorPairType>(ColorPair::BLACK_ON_WHITE)));
    m_parentWindow.bkgd(
        COLOR_PAIR(static_cast<ColorPairType>(ColorPair::BLACK_ON_WHITE)));

    // Print title
    m_parentWindow.color_set(
        static_cast<ColorPairType>(ColorPair::BLACK_ON_YELLOW));
    m_parentWindow.addstr(0, 3, m_title.c_str());

    // Set text and background color
    m_subWindow.color_set(static_cast<ColorPairType>(colorPair));
}

StatusStream Window::printLine()
{
    return StatusStream(*this);
}

MessageStream Window::display()
{
    return MessageStream(*this);
}

void Window::addString(std::string& string)
{
    // Print message to window
    m_subWindow.addstr(string.c_str());
}

void Window::showString(std::string& string)
{
    m_subWindow.clear();

    if (m_textPosition == TextPosition::LEFT)
    {
        m_subWindow.addstr(0, 0, string.c_str());
    }
    else // TextPosition::CENTER
    {
        auto heigth = m_subWindow.height();
        auto width = m_subWindow.width();
        auto centerHeight = heigth / 2;
        auto centerWidth = (width - string.size()) / 2;

        m_subWindow.addstr(centerHeight, centerWidth, string.c_str());
    }
}

bool Window::waitForExit()
{
    return m_subWindow.getch() != 'q';
}

} // namespace tui
} // namespace csf

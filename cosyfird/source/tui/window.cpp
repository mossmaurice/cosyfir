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
               const uint8_t textColor,
               const uint8_t textBackgroundColor)
    : m_parentWindow(lines, cols, beginX, beginY)
    , m_subWindow(m_parentWindow, true)
    , m_title(title)
    , m_textPosition(textPosition)
{

    // Refresh automatically
    m_subWindow.immedok(true);
    m_parentWindow.immedok(true);

    // Allow scrolling
    m_subWindow.scrollok(true);

    m_parentWindow.useColors();
    m_subWindow.useColors();

    // Set background colors
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    m_subWindow.bkgd(COLOR_PAIR(1));
    m_parentWindow.bkgd(COLOR_PAIR(1));

    // Print title
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    m_parentWindow.color_set(2);
    m_parentWindow.addstr(0, 3, m_title.c_str());

    // Set text and background color
    static uint8_t colorPairCount{3};
    init_pair(colorPairCount, textColor, textBackgroundColor);
    m_subWindow.color_set(colorPairCount);
    colorPairCount++;
}

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
}

void Window::showString(std::string& string)
{
    m_subWindow.clear();

    if (m_textPosition == TextPosition::LEFT)
    {
        m_subWindow.addstr(0, 0, string.c_str());
    }
    else
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

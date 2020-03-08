#include "tui/window.hpp"
namespace csf
{
namespace tui
{
Window::Window(std::string title, int lines, int cols, int beginX, int beginY)
    : NCursesColorWindow(lines, cols, beginX, beginY), m_title(title)
{
    // Refresh automatically
    immedok(true);

    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    bkgd(COLOR_PAIR(1));

    printBorder();

    /// @todo make this user configurable
    scrollok(true);

    move(1, 1);
}

Stream Window::print() { return Stream(*this); }

void Window::printBorder()
{
    // Save cursor position
    int x;
    int y;
    getyx(x, y);

    // Draw a box
    box();
    // Print title
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    color_set(2);
    addstr(0, 3, m_title.c_str());
    color_set(1);

    // Reset cursor position
    move(x, y);
}

} // namespace tui
} // namespace csf

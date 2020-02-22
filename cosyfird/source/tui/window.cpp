#include "tui/window.hpp"
namespace csf
{
namespace tui
{
Window::Window(int lines, int cols, int beginX, int beginY)
    : NCursesWindow(lines, cols, beginX, beginY)
{
    useColors();
    // Draw a box
    box();
}

Stream Window::stream() { return Stream(*this); }

} // namespace tui
} // namespace csf

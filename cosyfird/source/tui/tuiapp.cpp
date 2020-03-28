#include "tui/tuiapp.hpp"

namespace csf
{
namespace tui
{
App::App()
    : NCursesApplication(true)
{
    init_pair(greenOnWhite, COLOR_WHITE, COLOR_GREEN);
}

App::~App()
{
    endwin();
}

int App::run()
{
    return 0;
}

} // namespace tui
} // namespace csf

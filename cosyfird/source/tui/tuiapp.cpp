#include "tui/tuiapp.hpp"
namespace csf
{
namespace tui
{
App::App() : NCursesApplication(true) {}

App::~App() { endwin(); }

int App::run()
{

}

void App::addTitle()
{
    const std::string title = "Status log";

    titleWindow->bkgd(screen_titles());
    titleWindow->addstr(
        0, (titleWindow->cols() - title.length()) / 2, title.c_str());
    titleWindow->noutrefresh();
}
} // namespace tui
} // namespace csf


#include "tui/tuiapp.hpp"

using namespace csf;

int main(int argc, char* argv[]) try
{
    // Create container class for TUI
    tui::App cosyfirdApp;

    cosyfirdApp.handleArgs(argc, argv);

    // Start the application by calling the function
    // call operator which will call our run()
    return cosyfirdApp();
}
catch (const NCursesException& exception)
{
    std::cerr << "Problem with NCurses occured: " << exception.message
              << " Errno: " << exception.errorno << std::endl;
}

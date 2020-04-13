
#include "tui/tuiapp.hpp"

using namespace csf;

int main(int argc, char* argv[]) try
{
    // Create container class for TUI
    tui::App cosyfirdApp;

    /// @todo Exit with 'q', search paths for yaml file, add empty yaml template, add cmake install command
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


#include "tui/tuiapp.hpp"

using namespace csf;

int main(int argc, char* argv[])
{
    // Create container class for TUI
    tui::App cosyfirdApp;

    // Start the application by calling the function
    // call operator which will call our run()
    return cosyfirdApp();
}

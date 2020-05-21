// Copyright 2020 Simon Hoinkis
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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

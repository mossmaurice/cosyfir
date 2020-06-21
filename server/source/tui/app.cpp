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

#include "tui/app.hpp"
#include "version.h"

namespace csf
{
namespace tui
{
App::App()
    : NCursesApplication(true)
{
}

int App::titlesize() const
{
    return 1;
}

void App::printTitle(std::string appName)
{
    using ColorPairType = std::underlying_type<ColorPair>::type;

    m_titleWindow.immedok(true);
    m_titleWindow.color_set(
        static_cast<ColorPairType>(ColorPair::BLACK_ON_YELLOW));

    std::string title{" "};
    title.append(appName);
    title.append(" v");
    title.append(versionString);
    title.append(" ");
    m_titleWindow.addstr(title.c_str());
}

void App::init(bool enableColors)
{
    using ColorPairType = std::underlying_type<ColorPair>::type;

    if (enableColors)
    {
        NCursesWindow::useColors();

        init_pair(static_cast<ColorPairType>(ColorPair::BLACK_ON_WHITE),
                  COLOR_BLACK,
                  COLOR_WHITE);
        init_pair(static_cast<ColorPairType>(ColorPair::WHITE_ON_BLACK),
                  COLOR_WHITE,
                  COLOR_BLACK);
        init_pair(static_cast<ColorPairType>(ColorPair::BLACK_ON_YELLOW),
                  COLOR_BLACK,
                  COLOR_YELLOW);
        init_pair(static_cast<ColorPairType>(ColorPair::WHITE_ON_GREEN),
                  COLOR_WHITE,
                  COLOR_GREEN);

        bkgd(COLOR_PAIR(static_cast<ColorPairType>(ColorPair::WHITE_ON_BLACK)));
    }

    // Make cursor invisible
    curs_set(0);
}

void App::keepVisible()
{
    touchwin(stdscr);
    wrefresh(stdscr);
    m_titleWindow.touchwin();
    m_titleWindow.refresh();
}

} // namespace tui
} // namespace csf

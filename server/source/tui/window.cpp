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

#include "tui/window.hpp"
#include "tui/message_stream.hpp"
#include "tui/status_stream.hpp"

#include <thread>

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
               const ColorPair colorPair)
    : m_parentWindow(lines, cols, beginX, beginY)
    , m_subWindow(m_parentWindow, true)
    , m_title(title)
    , m_textPosition(textPosition)
    , m_lines(lines)
    , m_cols(cols)
{
    using ColorPairType = std::underlying_type<ColorPair>::type;

    // Refresh automatically
    m_subWindow.immedok(true);
    m_parentWindow.immedok(true);

    // Return immediately from getch() call
    m_subWindow.nodelay(true);

    // Allow scrolling
    m_subWindow.scrollok(true);

    // Use function keys
    m_subWindow.keypad(true);

    // Set background colors
    m_subWindow.bkgd(
        COLOR_PAIR(static_cast<ColorPairType>(ColorPair::BLACK_ON_WHITE)));
    m_parentWindow.bkgd(
        COLOR_PAIR(static_cast<ColorPairType>(ColorPair::BLACK_ON_WHITE)));

    printTitle();

    // Set text and background color
    m_subWindow.color_set(static_cast<ColorPairType>(colorPair));
}

void Window::printTitle()
{
    using ColorPairType = std::underlying_type<ColorPair>::type;

    // Print title
    m_parentWindow.color_set(
        static_cast<ColorPairType>(ColorPair::BLACK_ON_YELLOW));
    m_parentWindow.addstr(0, 3, m_title.c_str());
    m_parentWindow.color_set(
        static_cast<ColorPairType>(ColorPair::BLACK_ON_WHITE));
}

StatusStream Window::printLine()
{
    return StatusStream(*this);
}

MessageStream Window::display()
{
    return MessageStream(*this);
}

void Window::addString(std::string& string)
{
    // Print message to window
    m_subWindow.addstr(string.c_str());
}

void Window::showString(std::string& string)
{
    m_subWindow.clear();

    if (m_textPosition == TextPosition::LEFT)
    {
        m_subWindow.addstr(string.c_str());
    }
    else // TextPosition::CENTER
    {
        auto heigth = m_subWindow.height();
        auto width = m_subWindow.width();
        auto centerHeight = heigth / 2;
        auto centerWidth = (width - string.size()) / 2;

        m_subWindow.addstr(centerHeight, centerWidth, string.c_str());
    }
}

void Window::waitForExit()
{
    using namespace std::chrono_literals;

    while (m_subWindow.getch() != 'q')
    {
        std::this_thread::sleep_for(100ms);
    }
}

int Window::getKeyStroke()
{
    return m_subWindow.getch();
}

void Window::keepVisible()
{
    m_parentWindow.wresize(m_lines, m_cols);
    m_parentWindow.box();
    printTitle();
    m_parentWindow.touchwin();
    m_parentWindow.refresh();
    m_subWindow.wresize(m_lines - 2, m_cols - 2);
    m_subWindow.touchwin();
    m_subWindow.refresh();
}

} // namespace tui
} // namespace csf

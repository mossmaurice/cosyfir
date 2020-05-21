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

#pragma once

#include "tui/tuiapp.hpp"

#include "cursesw.h"

namespace csf
{
namespace tui
{

class StatusStream;
class MessageStream;

enum class TextPosition
{
    LEFT,
    CENTER
};

class Window
{
  public:
    Window(const std::string title,
           const uint16_t lines,
           const uint16_t cols,
           const uint16_t beginX,
           const uint16_t beginY,
           const TextPosition = TextPosition::LEFT,
           const ColorPair colorPair = ColorPair::BLACK_ON_WHITE);
    virtual ~Window() = default;
    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

    /// @brief Prints a line with date
    StatusStream printLine();

    /// @brief Displays stream, replacing old content
    MessageStream display();

    /// @brief Prints string at current cursor position
    void addString(std::string& string);

    /// @brief Clears the window and displays the string
    void showString(std::string& string);

    /// @brief Waits till 'q' key was pressed
    void waitForExit();

    /// @brief Returns user input
    int getKeyStroke();

  private:
    NCursesWindow m_parentWindow;
    NCursesWindow m_subWindow;
    std::string m_title;
    TextPosition m_textPosition;
};
} // namespace tui
} // namespace csf

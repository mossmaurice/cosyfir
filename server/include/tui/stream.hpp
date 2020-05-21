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

#include "tui/window.hpp"

#include <chrono>
#include <sstream>
#include <string>

namespace csf
{
namespace tui
{

class Window;

/// @brief Prints a stream to a specific Window
class Stream
{
  public:
    Stream(Window& window);
    virtual ~Stream() = default;
    Stream(const Stream&) = default;
    Stream(Stream&&) = default;
    Stream& operator=(const Stream&) = default;
    Stream& operator=(Stream&&) = default;

    Stream& operator<<(const char* value);
    Stream& operator<<(const std::string& string);
    Stream& operator<<(const std::stringstream& sstring);

    template <typename T,
              typename = std::enable_if_t<std::is_arithmetic<T>::value>>
    Stream& operator<<(const T& value)
    {
        std::stringstream sstream;
        sstream << std::to_string(value);
        m_message.append(sstream.str());
        return *this;
    }

  protected:
    Window& m_window;

    using SysClock = std::chrono::system_clock;
    using TimePoint = std::chrono::time_point<SysClock>;

    TimePoint m_timePoint{SysClock::now()};
    std::string m_message;
};

} // namespace tui
} // namespace csf

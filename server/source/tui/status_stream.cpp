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

#include "tui/status_stream.hpp"

#include <iomanip>

namespace csf
{
namespace tui
{

StatusStream::StatusStream(Window& window)
    : Stream(window)
{
}

StatusStream::~StatusStream()
{
    m_message.append("\n");

    // Add date and time to output
    std::stringstream strMessage;
    std::time_t time = std::chrono::system_clock::to_time_t(m_timePoint);
    strMessage << "[" << std::put_time(std::localtime(&time), "%F %T") << "]"
               << " " << m_message;
    std::string messageWithDate = strMessage.str();

    m_window.addString(messageWithDate);
}

} // namespace tui
} // namespace csf

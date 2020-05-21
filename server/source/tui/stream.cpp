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

#include "tui/stream.hpp"

#include <iomanip>

namespace csf
{
namespace tui
{

Stream::Stream(Window& window)
    : m_window(window)
{
}

Stream& Stream::operator<<(const char* charString)
{
    m_message.append(charString);
    return *this;
}

Stream& Stream::operator<<(const std::string& string)
{
    m_message.append(string);
    return *this;
}

Stream& Stream::operator<<(const std::stringstream& sstring)
{
    m_message.append(sstring.str());
    return *this;
}

} // namespace tui
} // namespace csf

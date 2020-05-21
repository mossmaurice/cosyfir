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

#include <cursesapp.h>
#include <thread>
namespace csf
{
namespace tui
{

enum class ColorPair : uint16_t
{
    BLACK_ON_WHITE = 1,
    WHITE_ON_BLACK,
    BLACK_ON_YELLOW,
    WHITE_ON_GREEN
};

class App : public NCursesApplication
{
  public:
    App();
    ~App() = default;
    App(const App&) = delete;
    App(App&&) = delete;
    App& operator=(const App&) = delete;
    App& operator=(App&&) = delete;

    void handleArgs(int argc, char* argv[]) override;

  private:
    int titlesize() const override;
    void title() override;
    int run() override;
    void init(bool enableColors) override;
    bool m_running{true};
    std::string defaultConfigPath{"/etc/cosyfir/cosyfird.yaml"};
};
} // namespace tui
} // namespace csf

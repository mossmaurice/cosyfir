#pragma once

#include <cursesapp.h>

namespace csf
{
namespace tui
{
class App : public NCursesApplication
{
  public:
    App();
    virtual ~App();
    App(const App&) = delete;
    App(App&&) = delete;
    App& operator=(const App&) = delete;
    App& operator=(App&&) = delete;

    int run() override;

    uint8_t greenOnWhite{3};
};
} // namespace tui
} // namespace csf

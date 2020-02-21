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

  private:
    void addTitle();
};
} // namespace tui
} // namespace csf

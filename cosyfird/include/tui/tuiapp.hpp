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
    virtual ~App();
    App(const App&) = delete;
    App(App&&) = delete;
    App& operator=(const App&) = delete;
    App& operator=(App&&) = delete;

  private:
    int run() override;
    void makeColorPairs();
    bool m_running{true};
};
} // namespace tui
} // namespace csf

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

    int titlesize() const override;
    void title() override;

    void handleArgs(int argc, char* argv[]) override;

  private:
    int run() override;
    void init(bool enableColors) override;
    bool m_running{true};
    std::string defaultConfigPath{"/etc/cosyfir/cosyfird.yaml"};
};
} // namespace tui
} // namespace csf

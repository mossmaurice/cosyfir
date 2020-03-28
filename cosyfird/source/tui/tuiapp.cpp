#include "tui/tuiapp.hpp"
#include "cosyfir-sub/configparser.hpp"
#include "network/mqttclient.hpp"
#include "tui/window.hpp"

#include <chrono>
#include <exception>
#include <optional>
#include <thread>

namespace csf
{
namespace tui
{
App::App()
    : NCursesApplication(true)
{
}

App::~App()
{
    endwin();
}

void App::makeColorPairs()
{
    using ColorPairType = std::underlying_type<ColorPair>::type;

    init_pair(static_cast<ColorPairType>(ColorPair::BLACK_ON_WHITE),
              COLOR_BLACK,
              COLOR_WHITE);
    init_pair(static_cast<ColorPairType>(ColorPair::WHITE_ON_BLACK),
              COLOR_WHITE,
              COLOR_BLACK);
    init_pair(static_cast<ColorPairType>(ColorPair::BLACK_ON_YELLOW),
              COLOR_BLACK,
              COLOR_YELLOW);
    init_pair(static_cast<ColorPairType>(ColorPair::WHITE_ON_GREEN),
              COLOR_WHITE,
              COLOR_GREEN);
}

int App::run()
{
    using namespace std::chrono_literals;
    constexpr char yamlFile[] = "cosyfird.yaml";

    makeColorPairs();

    // Create left window
    tui::Window statusWindow{" Log ", 30, 62, 3, 5};

    // Create window for LoRa payload
    tui::Window payloadWindow{" Last payload (hex) ",
                              10,
                              60,
                              3,
                              70,
                              tui::TextPosition::CENTER,
                              ColorPair::WHITE_ON_GREEN};

    // Create right window with full MQTT message
    tui::Window messageWindow{" Last full MQTT message ", 18, 60, 15, 70};

    // Parse yaml config file
    statusWindow.printLine() << "Reading " << yamlFile << "..";
    ConfigParser mqttSettings{yamlFile};

    // Establish connecting to TTN server
    statusWindow.printLine() << "Connecting to TTN server..";

    /// @todo wrap this in an std::optional, nope we're using exceptions
    network::MqttClient client{mqttSettings.getHostAddress(),
                               mqttSettings.getPort(),
                               mqttSettings.getClientId(),
                               mqttSettings.getPassword(),
                               statusWindow,
                               messageWindow,
                               payloadWindow};

    while (statusWindow.waitForExit())
    {
        if (client.loop()
            != MOSQ_ERR_SUCCESS) // @todo maybe put this to a thread
        {
            statusWindow.printLine() << "Network problem, please restart";
        }
        else
        {
            statusWindow.printLine() << "Loop!";
        }
        std::this_thread::sleep_for(100ms);
    }

    return 0;
}

} // namespace tui
} // namespace csf

#include "tui/tuiapp.hpp"
#include "cosyfird/configparser.hpp"
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

/// @todo move this to init method
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

    // Make cursor invisible
    curs_set(0);

    // Create left window
    tui::Window statusWindow{" Log ", 30, 62, 3, 5};

    // Create window for LoRa payload
    tui::Window payloadWindow{" Last payload (hex) ",
                              6,
                              60,
                              3,
                              70,
                              tui::TextPosition::CENTER,
                              ColorPair::WHITE_ON_GREEN};

    // Create right window with full MQTT message
    tui::Window messageWindow{" Last full MQTT message ", 22, 60, 11, 70};

    try
    {
        // Read config file
        ConfigParser mqttSettings{statusWindow, yamlFile};

        // Establish connecting to TTN server
        network::MqttClient client{mqttSettings.getMqttConfig(),
                                   statusWindow,
                                   messageWindow,
                                   payloadWindow};

        auto networkLoop = [&]() {
            while (m_running)
            {
                if (client.loop() != MOSQ_ERR_SUCCESS)
                {
                    statusWindow.printLine()
                        << "Network problem, please restart";
                }
                std::this_thread::sleep_for(100ms);
            }
        };

        std::thread networkThread{networkLoop};

        while (statusWindow.waitForExit())
        {
        }

        m_running = false;
        if (networkThread.joinable())
        {
            networkThread.join();
        }

        statusWindow.printLine() << "Stopping gracefully..";
        std::this_thread::sleep_for(3s);
        return EXIT_SUCCESS;
    }
    catch (const std::exception& exception)
    {
        statusWindow.printLine() << "Press 'q' to exit";
        while (statusWindow.waitForExit())
        {
        }
        return EXIT_FAILURE;
    }
}

} // namespace tui
} // namespace csf

#include "cosyfir-sub/configparser.hpp"
#include "network/mqttclient.hpp"
#include "tui/tuiapp.hpp"
#include "tui/window.hpp"

#include <chrono>
#include <cursesapp.h>
#include <exception>
#include <optional>
#include <thread>

using namespace csf;

int main()
{
    using namespace std::chrono_literals;
    constexpr char yamlFile[] = "cosyfird.yaml";

    // Create container class for TUI
    tui::App cosyfirdApp;

    // Create left window
    tui::Window statusWindow{" Log ", 30, 62, 3, 5};

    // Create window for LoRa payload
    tui::Window payloadWindow{" Payload (hex) ", 10, 60, 3, 70};

    // Create right window with full MQTT message
    tui::Window messageWindow{" Full MQTT message ", 18, 60, 15, 70};

    // Print to left window
    statusWindow.printLine() << "Reading " << yamlFile << "..";
    ConfigParser mqttSettings{yamlFile};

    // Print to left window
    statusWindow.printLine() << "Connecting to TTN server..";

    /// @todo wrap this in an std::optional
    network::MqttClient client{mqttSettings.getHostAddress(),
                               mqttSettings.getPort(),
                               mqttSettings.getClientId(),
                               mqttSettings.getPassword(),
                               statusWindow,
                               messageWindow,
                               payloadWindow};

    while (statusWindow.getch() != 'q')
    {
        if (client.loop()
            != MOSQ_ERR_SUCCESS) // @todo maybe put this to a thread
        {
            statusWindow.printLine() << "Could not loop!";
        }
        else
        {
            statusWindow.printLine() << "Loop!";
        }
        std::this_thread::sleep_for(100ms);
    }

    return cosyfirdApp.run();
}

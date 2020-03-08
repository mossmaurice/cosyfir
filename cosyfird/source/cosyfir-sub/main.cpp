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
    tui::Window statusWindow{" Log ", 30, 60, 3, 5};

    // Create window for LoRa payload
    tui::Window payloadWindow{
        " Payload (hex) ", 10, 50, 3, 70}; // @todo mabye tui::HexWindow?

    // Create right window with full MQTT message
    tui::Window messageWindow{" Full MQTT message ", 18, 50, 15, 70};

    // Print to left window
    statusWindow.print() << "Reading " << yamlFile << "..";
    ConfigParser mqttSettings{yamlFile};

    // Print to left window
    statusWindow.print() << "Connecting to TTN server..";

    /// @todo wrap this in an std::optional
    network::MqttClient client{mqttSettings.getHostAddress(),
                               mqttSettings.getPort(),
                               mqttSettings.getClientId(),
                               mqttSettings.getPassword(),
                               statusWindow,
                               messageWindow};

    while (statusWindow.getch() != 'q')
    {
        if (client.loop()
            != MOSQ_ERR_SUCCESS) // @todo maybe put this to a thread
        {
            statusWindow.print() << "Could not loop!";
        }
        else
        {
            statusWindow.print() << "Loop!";
        }
        std::this_thread::sleep_for(100ms);
    }

    return cosyfirdApp.run();
}

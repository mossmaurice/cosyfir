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
    tui::Window statusWindow{70, 50, 3, 5};

    // Create right window with full MQTT message
    tui::Window messageWindow{70, 50, 10, 60};

    // Create window for LoRa payload
    tui::Window payloadWindow{20, 50, 3, 60};

    // Print to left window
    statusWindow.stream() << "Reading " << yamlFile << "..";
    ConfigParser mqttSettings{yamlFile};

    // Print to left window
    statusWindow.stream() << "Connecting to TTN server..";

    /// @todo wrap this in an std::optional
    network::MqttClient client{mqttSettings.getHostAddress(),
                               mqttSettings.getPort(),
                               mqttSettings.getClientId(),
                               mqttSettings.getPassword(),
                               statusWindow,
                               messageWindow};

    /// @todo while(getchar("q"))
    while (1)
    {
        if (client.loop() != MOSQ_ERR_SUCCESS)
        {
            statusWindow.stream() << "Could not loop!";
        }
        else
        {
            statusWindow.stream() << "Loop!";
        }
        std::this_thread::sleep_for(100ms);
    }

    return cosyfirdApp.run();
}

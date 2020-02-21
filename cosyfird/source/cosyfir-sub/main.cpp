#include "cosyfir-sub/configparser.hpp"
#include "network/mqttclient.hpp"
#include "tui/tuiapp.hpp"
#include "tui/window.hpp"

#include <chrono>
#include <exception>
#include <cursesapp.h>
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
    tui::Window statusWindow;

    // Create right window
    tui::Window messageWindow;

    initscr();

    // Print to left window
    printw("Reading %s..\n", yamlFile);
    ConfigParser mqttSettings{yamlFile};

    // Print to left window
    printw("Connecting to TTN server..\n");
    refresh();

    /// @todo wrap this in an std::optional
    MqttClient client{mqttSettings.getHostAddress(),
                      mqttSettings.getPort(),
                      mqttSettings.getClientId(),
                      mqttSettings.getPassword()};

    /// @todo while(getchar("q"))
    while (1)
    {
        if (client.loop() != MOSQ_ERR_SUCCESS)
        {
            printw("Could not loop!\n");
            refresh();
        }
        else
        {
            printw("Loop!\n");
            refresh();
        }
        std::this_thread::sleep_for(100ms);
    }

    return 0;
}

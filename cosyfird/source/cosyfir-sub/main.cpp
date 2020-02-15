#include "cosyfir-sub/configparser.hpp"
#include "network/mqttclient.hpp"

#include <chrono>
#include <exception>
/// @todo Try ncurses c++ https://reversed.top/2016-04-24/ncurses-for-cpp/
#include <cursesapp.h>
#include <optional>
#include <thread>

int main()
{
    using namespace std::chrono_literals;

    constexpr char yamlFile[] = "cosyfird.yaml";

    initscr();
    printw("Reading %s..\n", yamlFile);

    ConfigParser mqttSettings{yamlFile};

    printw("Connecting to TTN server..\n");
    refresh();

    /// @todo wrap this in an std::optional
    MqttClient client{mqttSettings.getHostAddress(),
                      mqttSettings.getPort(),
                      mqttSettings.getClientId(),
                      mqttSettings.getPassword()};

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

    getch();
    endwin();

    return 0;
}

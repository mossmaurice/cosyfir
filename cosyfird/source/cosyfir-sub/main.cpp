#include "network/mqttclient.hpp"
#include "cosyfir-sub/configparser.hpp"

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
    MqttClient client;

    /// @todo move the following four calls to c'tor of MqttClient
    if (client.username_pw_set(mqttSettings.getClientId().c_str(), mqttSettings.getPassword().c_str()) != MOSQ_ERR_SUCCESS)
    {
        printw("Could not connect!\n");
        refresh();
    }

    if (client.tls_set(PEM_FILE, nullptr, nullptr, nullptr, nullptr)
        != MOSQ_ERR_SUCCESS)
    {
        printw("Could not set TLS parameters!\n");
        refresh();
    }

    if (client.connect(mqttSettings.getHostAddress().c_str(), mqttSettings.getPort()) != MOSQ_ERR_SUCCESS)
    {
        printw("Could not connect!\n");
        refresh();
    }

    if (client.subscribe(nullptr, "+/devices/+/up", 0) != MOSQ_ERR_SUCCESS)
    {
        printw("Could not subscribe!\n");
        refresh();
    }

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

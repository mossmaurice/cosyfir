#include <chrono>
#include <exception>
#include <mosquittopp.h>
/// @todo Try ncurses c++ https://reversed.top/2016-04-24/ncurses-for-cpp/
#include <cursesapp.h>
#include <optional>
#include <thread>

int main()
{
    using namespace std::chrono_literals;

    initscr();
    printw("Connecting to TTN server..\n");
    refresh();

    constexpr char CLIENT_ID[] = "foo";
    constexpr char HOST_ADDR[] = "eu.thethings.network";
    constexpr uint16_t PORT = 8883;
    constexpr char PASSWD[] = "bar";
    constexpr char PEM_FILE[] = "./mqtt-ca.pem";
    constexpr char PEM_PATH[] = "foo";

    if (mosqpp::lib_init() != mosq_err_t::MOSQ_ERR_SUCCESS)
    {
        // Early exit
        std::terminate();
    }

    /// @todo wrap this in an std::optional
    mosqpp::mosquittopp client;

    if (client.username_pw_set(CLIENT_ID, PASSWD) != MOSQ_ERR_SUCCESS)
    {
        printw("Could not connect!");
        refresh();
    }

    if (client.tls_set(PEM_FILE, PEM_PATH, nullptr, nullptr, nullptr)
        != MOSQ_ERR_SUCCESS)
    {
        printw("Could not set TLS parameters!");
        refresh();
    }

    // Connect to TTN server
    if (client.connect(HOST_ADDR, PORT) != MOSQ_ERR_SUCCESS)
    {
        printw("Could not connect!");
        refresh();
    }

    if (client.subscribe(nullptr, "+/devices/+/up", 0) != MOSQ_ERR_SUCCESS)
    {
        printw("Could not subscribe!");
        refresh();
    }

    while (1)
    {
        if (client.loop() != MOSQ_ERR_SUCCESS)
        {
            printw("Could not loop! \n");
            refresh();
        }
        else
        {
            printw("Loop! \n");
            refresh();
        }
        std::this_thread::sleep_for(100ms);
    }

    if (mosqpp::lib_cleanup() != MOSQ_ERR_SUCCESS)
    {
        std::terminate();
    }

    getch();
    endwin();

    return 0;
}

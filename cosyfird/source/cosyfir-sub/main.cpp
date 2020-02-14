#include <chrono>
#include <exception>
#include <mosquittopp.h>
/// @todo Try ncurses c++ https://reversed.top/2016-04-24/ncurses-for-cpp/
#include <cursesapp.h>
#include <optional>
#include <thread>

class MqttClient : public mosqpp::mosquittopp
{
  public:
    MqttClient()
    {
        if (mosqpp::lib_init() != mosq_err_t::MOSQ_ERR_SUCCESS)
        {
            // Early exit
            std::terminate();
        }
    }

    ~MqttClient()
    {
        if (mosqpp::lib_cleanup() != MOSQ_ERR_SUCCESS)
        {
            std::terminate();
        }
    }

    MqttClient(const MqttClient &) = delete;
    MqttClient(MqttClient &&) = delete;
    MqttClient &operator=(const MqttClient &) = delete;
    MqttClient &operator=(MqttClient &&) = delete;

    void on_message(const struct mosquitto_message *message) override
    {
        if (message)
        {
            printw("New message arrived!\n");
            printw("mid: %d \n", message->mid);
            printw("topic: %c \n", message->topic);
            printw("payload: 0x%x \n", message->payload);
            refresh();
        }
    }
};

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
    /// @note TTN is using Let's Encrypt, let's hardcode their root cert for the
    /// time being
    constexpr char PEM_FILE[] = "/etc/ssl/certs/DST_Root_CA_X3.pem";

    /// @todo wrap this in an std::optional
    MqttClient client;

    if (client.username_pw_set(CLIENT_ID, PASSWD) != MOSQ_ERR_SUCCESS)
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

    // Connect to TTN server
    if (client.connect(HOST_ADDR, PORT) != MOSQ_ERR_SUCCESS)
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

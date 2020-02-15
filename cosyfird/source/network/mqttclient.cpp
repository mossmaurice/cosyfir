#include "network/mqttclient.hpp"

#include <cursesapp.h>

MqttClient::MqttClient(const std::string& hostAddress,
                       const uint16_t& port,
                       const std::string& clientId,
                       const std::string& password)
{
    if (mosqpp::lib_init() != mosq_err_t::MOSQ_ERR_SUCCESS)
    {
        // Early exit
        std::terminate();
    }

    if (username_pw_set(clientId.c_str(), password.c_str()) != MOSQ_ERR_SUCCESS)
    {
        printw("Could not connect!\n");
        refresh();
    }

    if (tls_set(PEM_FILE, nullptr, nullptr, nullptr, nullptr)
        != MOSQ_ERR_SUCCESS)
    {
        printw("Could not set TLS parameters!\n");
        refresh();
    }

    if (connect(hostAddress.c_str(), port) != MOSQ_ERR_SUCCESS)
    {
        printw("Could not connect!\n");
        refresh();
    }

    if (subscribe(nullptr, "+/devices/+/up", 0) != MOSQ_ERR_SUCCESS)
    {
        printw("Could not subscribe!\n");
        refresh();
    }
}

MqttClient::~MqttClient()
{
    if (mosqpp::lib_cleanup() != MOSQ_ERR_SUCCESS)
    {
        printw("There was problem with cleaning up mosquittopp!\n");
        std::terminate();
    }
}

void MqttClient::on_message(const struct mosquitto_message* message)
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

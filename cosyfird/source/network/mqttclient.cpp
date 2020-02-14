#include "network/mqttclient.hpp"

#include <cursesapp.h>

MqttClient::MqttClient()
{
    if (mosqpp::lib_init() != mosq_err_t::MOSQ_ERR_SUCCESS)
    {
        // Early exit
        std::terminate();
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

void MqttClient::on_message(const struct mosquitto_message *message)
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

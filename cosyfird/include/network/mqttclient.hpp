#pragma once

#include <mosquittopp.h>

class MqttClient : public mosqpp::mosquittopp
{
  public:
    MqttClient();

    ~MqttClient();

    MqttClient(const MqttClient &) = delete;
    MqttClient(MqttClient &&) = delete;
    MqttClient &operator=(const MqttClient &) = delete;
    MqttClient &operator=(MqttClient &&) = delete;

    void on_message(const struct mosquitto_message *message) override;
};
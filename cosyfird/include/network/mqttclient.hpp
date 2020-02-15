#pragma once

#include <mosquittopp.h>

/// @note TTN is using Let's Encrypt, let's hardcode their root cert for the
/// time being
constexpr char PEM_FILE[] = "/etc/ssl/certs/DST_Root_CA_X3.pem";

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
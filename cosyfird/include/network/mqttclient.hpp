#pragma once

#include <mosquittopp.h>
#include <string>

/// @note TTN is using Let's Encrypt, let's hardcode their root cert for the
/// time being
constexpr char PEM_FILE[] = "/etc/ssl/certs/DST_Root_CA_X3.pem";

namespace csf
{
namespace network
{
class MqttClient : public mosqpp::mosquittopp
{
  public:
    MqttClient(const std::string& hostAddress,
               const uint16_t& port,
               const std::string& clientId,
               const std::string& password);

    virtual ~MqttClient();

    MqttClient(const MqttClient&) = delete;
    MqttClient(MqttClient&&) = delete;
    MqttClient& operator=(const MqttClient&) = delete;
    MqttClient& operator=(MqttClient&&) = delete;

    void on_message(const struct mosquitto_message* message) override;
};
} // namespace network
} // namespace csf

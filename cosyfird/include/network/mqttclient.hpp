#pragma once

#include "tui/window.hpp"

#include <mosquittopp.h>
#include <string>
#include <vector>

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
               const std::string& password,
               tui::Window& statusWindow,
               tui::Window& messageWindow,
               tui::Window& payloadWindow);

    virtual ~MqttClient();

    MqttClient(const MqttClient&) = delete;
    MqttClient(MqttClient&&) = delete;
    MqttClient& operator=(const MqttClient&) = delete;
    MqttClient& operator=(MqttClient&&) = delete;

    void on_message(const struct mosquitto_message* message) override;

    tui::Window& m_statusWindow;
    tui::Window& m_messageWindow;
    tui::Window& m_payloadWindow;

    std::vector<std::string> m_topics{
        "+/devices/+/up",     // Uplink messages from any node
        "+/devices/+/events/" // Events from any node
    };

    /// @todo all relevant topics added?
    /// activations "my-app-id/devices/my-dev-id/events/activations"
    /// Uplink Errors: <AppID>/devices/<DevID>/events/up/errors
    /// Downlink Errors: <AppID>/devices/<DevID>/events/down/errors
    /// Activation Errors: <AppID>/devices/<DevID>/events/activations/errors
    /// Downlink Acknowledgements: <AppID>/devices/<DevID>/events/down/acks
    /// Created: <AppID>/devices/<DevID>/events/create
    /// Updated: <AppID>/devices/<DevID>/events/update
    /// Deleted: <AppID>/devices/<DevID>/events/delete
    /// Downlink Scheduled: <AppID>/devices/<DevID>/events/down/scheduled
    /// Downlink Sent: <AppID>/devices/<DevID>/events/down/sent
};
} // namespace network
} // namespace csf

// Copyright 2020 Simon Hoinkis
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
struct MqttConfig;
namespace network
{
class MqttClient : public mosqpp::mosquittopp
{
  public:
    MqttClient(const MqttConfig& mqttConfig,
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

    std::vector<std::string> m_genericTopics{
        /// @note
        /// AppID/devices/DevID/EventType
        /// "+" means match all
        "+/devices/+/up",                 // Uplink messages from any node
        "+/devices/+/down",               // Downlink messages to any node
        "+/devices/+/events/activations", // Activations from any node
        "+/devices/+/events/down/sent", // A scheduled downlink message was sent
                                        // to node
        "+/devices/+/events/down/acks"  // Acknowledge of downlink message
                                        // received

    };
};
} // namespace network
} // namespace csf

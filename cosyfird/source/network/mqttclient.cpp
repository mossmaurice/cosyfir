#include "network/mqttclient.hpp"

#include <b64/decode.h>
#include <cursesapp.h>
#include <iostream>
#include <jsoncpp/json/json.h>
#include <memory>

namespace csf
{
namespace network
{
/// @todo create a struct of first four params
MqttClient::MqttClient(const std::string& hostAddress,
                       const uint16_t& port,
                       const std::string& clientId,
                       const std::string& password,
                       tui::Window& statusWindow,
                       tui::Window& messageWindow,
                       tui::Window& payloadWindow)
    : m_statusWindow(statusWindow)
    , m_messageWindow(messageWindow)
    , m_payloadWindow(payloadWindow)
{
    if (mosqpp::lib_init() != mosq_err_t::MOSQ_ERR_SUCCESS)
    {
        // Early exit
        std::terminate();
    }

    if (username_pw_set(clientId.c_str(), password.c_str()) != MOSQ_ERR_SUCCESS)
    {
        m_statusWindow.printLine() << "Could not connect";
    }

    if (tls_set(PEM_FILE, nullptr, nullptr, nullptr, nullptr)
        != MOSQ_ERR_SUCCESS)
    {
        m_statusWindow.printLine() << "Could not set TLS parameters";
    }

    if (connect(hostAddress.c_str(), port) != MOSQ_ERR_SUCCESS)
    {
        m_statusWindow.printLine() << "Could not connect";
    }

    /// @todo make this a for loop over an array of strings
    m_statusWindow.printLine() << "Subscribing to +/devices/+/up..";
    if (subscribe(nullptr, "+/devices/+/up", 0) != MOSQ_ERR_SUCCESS)
    {
        m_statusWindow.printLine() << "Could not subscribe";
    }

    /// @todo subscribe to all topics
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
}

MqttClient::~MqttClient()
{
    if (mosqpp::lib_cleanup() != MOSQ_ERR_SUCCESS)
    {
        m_statusWindow.printLine()
            << "There was problem with cleaning up mosquittopp";
        std::terminate();
    }
}

void MqttClient::on_message(const struct mosquitto_message* message)
{
    if (message)
    {
        const std::string topic{message->topic};
        const std::string payloadJson{static_cast<char*>(message->payload)};

        // Read sensor node name
        auto last = topic.rfind("/");
        auto first = topic.rfind("/", last - 1);
        auto length = last - first;
        std::string nodeName{topic.substr(first + 1, length - 1)};

        m_statusWindow.printLine() << "Uplink message from " << nodeName;

        JSONCPP_STRING err;
        Json::Value jsonRoot;
        Json::CharReaderBuilder readerBuilder;
        const std::unique_ptr<Json::CharReader> reader(
            readerBuilder.newCharReader());

        if (!reader->parse(payloadJson.c_str(),
                           payloadJson.c_str() + payloadJson.length(),
                           &jsonRoot,
                           &err))
        {
            // Error occured
            std::terminate();
        }

        m_messageWindow.display() << jsonRoot.toStyledString();

        base64::decoder base64Decoder;

        std::string payloadBase64{jsonRoot["payload_raw"].asString()};

        std::stringstream in;
        std::stringstream out;

        in << payloadBase64;

        base64Decoder.decode(in, out);

        char bytes[32];
        out.read(bytes, sizeof(bytes));
        auto readBytes = out.gcount();
        if (readBytes > 8)
        {
            //m_statusWindow.printLine() << "Warning: Received " << readBytes
            //                           << " bytes but printed only 8";
        }
        m_payloadWindow.display()
            << " | " << bytes[0] << " | " << bytes[1] << " | " << bytes[2]
            << " | " << bytes[3] << " | " << bytes[4] << " | " << bytes[5]
            << " | " << bytes[6] << " | " << bytes[7] << " | ";
    }
}
} // namespace network
} // namespace csf

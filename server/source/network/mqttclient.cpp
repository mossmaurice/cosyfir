#include "network/mqttclient.hpp"
#include "cosyfird/configparser.hpp"
#include "tui/message_stream.hpp"
#include "tui/status_stream.hpp"

#include <b64/decode.h>
#include <cursesapp.h>
#include <iomanip>
#include <ios>
#include <iostream>
#include <jsoncpp/json/json.h>
#include <memory>

namespace csf
{
namespace network
{
MqttClient::MqttClient(const MqttConfig& mqttConfig,
                       tui::Window& statusWindow,
                       tui::Window& messageWindow,
                       tui::Window& payloadWindow)
    : m_statusWindow(statusWindow)
    , m_messageWindow(messageWindow)
    , m_payloadWindow(payloadWindow)
{
    statusWindow.printLine() << "Connecting to TTN server..";

    if (mosqpp::lib_init() != mosq_err_t::MOSQ_ERR_SUCCESS)
    {
        m_statusWindow.printLine() << "Initalization of mosquittopp failed";
        throw;
    }

    if (username_pw_set(mqttConfig.clientId.c_str(),
                        mqttConfig.password.c_str())
        != MOSQ_ERR_SUCCESS)
    {
        m_statusWindow.printLine() << "Could not set password and username";
        throw;
    }

    if (tls_set(PEM_FILE, nullptr, nullptr, nullptr, nullptr)
        != MOSQ_ERR_SUCCESS)
    {
        m_statusWindow.printLine() << "Could not set TLS parameters";
        throw;
    }

    if (connect(mqttConfig.hostAddress.c_str(), mqttConfig.port)
        != MOSQ_ERR_SUCCESS)
    {
        m_statusWindow.printLine() << "Could not connect to server";
        throw;
    }

    m_statusWindow.printLine() << "Sucessfully connected with TTN server";

    for (auto& topic : m_genericTopics)
    {
        m_statusWindow.printLine()
            << "Subscribing to '" << topic.c_str() << "'..";
        if (subscribe(nullptr, topic.c_str(), 0) != MOSQ_ERR_SUCCESS)
        {
            m_statusWindow.printLine()
                << "Could not subscribe to " << topic.c_str();
            throw;
        }
    }
}

MqttClient::~MqttClient()
{
    if (mosqpp::lib_cleanup() != MOSQ_ERR_SUCCESS)
    {
        m_statusWindow.printLine()
            << "There was problem with cleaning up mosquittopp";
    }
}

void MqttClient::on_message(const struct mosquitto_message* message)
{
    if (message)
    {
        const std::string topic{message->topic};
        const std::string payloadJson{static_cast<char*>(message->payload)};

        m_statusWindow.printLine()
            << "Received message on topic '" << topic << "'";

        /// @todo Create iceoryx publisher with received topic here if it wasn't
        /// found in std::map of publishers

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
            throw;
        }

        m_messageWindow.display() << jsonRoot.toStyledString();

        base64::decoder base64Decoder;

        std::string payloadBase64{jsonRoot["payload_raw"].asString()};

        std::stringstream in;
        std::stringstream out;

        in << payloadBase64;

        base64Decoder.decode(in, out);

        constexpr uint8_t BYTE_COUNT{8};

        /// @note ANDing the bitmask is needed due to the fact that chars are
        /// interpreted as characters and not uint8_t
        uint8_t bitmask = 0xff;

        std::stringstream sstring;
        sstring << std::uppercase << std::setfill('0') << std::setw(2)
                << std::hex << " | ";

        uint8_t validBytes{0};

        while (out.good())
        {
            auto value = (bitmask & out.get());
            if (out.gcount() > 0)
            {
                validBytes++;
                if (validBytes <= BYTE_COUNT)
                {
                    sstring << value << " | ";
                }
            }
        }

        m_payloadWindow.display() << sstring;

        if (validBytes > BYTE_COUNT)
        {
            m_statusWindow.printLine()
                << "Warning: Received " << validBytes
                << " bytes but printed only " << BYTE_COUNT;
        }
    }
}
} // namespace network
} // namespace csf

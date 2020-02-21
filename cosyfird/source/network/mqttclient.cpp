#include "network/mqttclient.hpp"

#include <b64/decode.h>
#include <cursesapp.h>
#include <iostream>
#include <jsoncpp/json/json.h>
#include <memory>

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
        // Print to left window
        printw("Could not connect!\n");
        refresh();
    }

    if (tls_set(PEM_FILE, nullptr, nullptr, nullptr, nullptr)
        != MOSQ_ERR_SUCCESS)
    {
        // Print to left window
        printw("Could not set TLS parameters!\n");
        refresh();
    }

    if (connect(hostAddress.c_str(), port) != MOSQ_ERR_SUCCESS)
    {
        // Print to left window
        printw("Could not connect!\n");
        refresh();
    }

    if (subscribe(nullptr, "+/devices/+/up", 0) != MOSQ_ERR_SUCCESS)
    {
        // Print to left window
        printw("Could not subscribe!\n");
        refresh();
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
        // Print to left window
        printw("There was problem with cleaning up mosquittopp!\n");
        std::terminate();
    }
}

void MqttClient::on_message(const struct mosquitto_message* message)
{
    if (message)
    {
        // Print to left window
        printw("New message arrived!\n");

        const std::string topic{message->topic};
        const std::string payloadJson{static_cast<char*>(message->payload)};

        JSONCPP_STRING err;
        Json::Value jsonRoot;
        Json::CharReaderBuilder builder;
        const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

        if (!reader->parse(payloadJson.c_str(),
                           payloadJson.c_str() + payloadJson.length(),
                           &jsonRoot,
                           &err))
        {
            // Error occured
            std::terminate();
        }

        base64::decoder base64Decoder;

        std::string payloadBase64{jsonRoot["payload_raw"].asString()};
        char decodedPayload[4];
        base64Decoder.decode(
            payloadBase64.c_str(), payloadBase64.length(), decodedPayload);

        // Print to right window
        std::cout << "sensorInstance: " << jsonRoot["dev_id"].asString() << endl;
        std::cout << "devEui: " << jsonRoot["hardware_serial"].asString() << endl;
        std::cout << "frameCounter: " << jsonRoot["counter"].asUInt() << endl;
        for (auto byte : decodedPayload)
        {
            printf("payload (hex): %02x", decodedPayload[byte]);
        }

        refresh();
    }
}

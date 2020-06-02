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

#include "cosyfird/app.hpp"
#include "cosyfird/configparser.hpp"
#include "network/mqttclient.hpp"
#include "tui/message_stream.hpp"
#include "tui/status_stream.hpp"
#include "tui/window.hpp"
#include "version.h"

#include <getopt.h>
#include <thread>

namespace csf
{
namespace daemon
{
void App::title()
{
    printTitle("cosyfird");
}

void App::handleArgs(int argc, char* argv[])
{
    constexpr const option longOpt[] = {
        {"help", no_argument, nullptr, 'h'},
        {"version", no_argument, nullptr, 'V'},
        {"config-file", required_argument, nullptr, 'c'},
        {0, 0, 0, 0}};
    constexpr char shortOpt[] = "hVc:";
    int opt{-1};
    int optIndex{0};

    while ((opt = getopt_long(argc, argv, shortOpt, longOpt, &optIndex)) != -1)
    {
        switch (opt)
        {
        case 'h':
            std::cout << "Usage is: cosyfird [options]" << std::endl;
            std::cout << "Available options are:" << std::endl;
            std::cout << "  -h, --help              Show this usage output"
                      << std::endl;
            std::cout << "  -V, --version           Show version and build time"
                      << std::endl;
            std::cout << "  -c, --config-file=FILE  Use given config file"
                      << std::endl;
            std::cout << "                          Default path "
                         "is '/etc/cosyfir/cosyfird.yaml'"
                      << std::endl;
            m_running = false;
            break;

        case 'V':
            std::cout << "Version:             " << versionString << std::endl;
            std::cout << "Build date and time: " << buildDateTimeString
                      << std::endl;
            m_running = false;
            break;

        case 'c':
            defaultConfigPath.clear();
            defaultConfigPath = optarg;
            break;

        default:
            // Do not start the app if an argument isn't known
            m_running = false;
            break;
        }
    }
}

int App::run()
{
    using namespace std::chrono_literals;

    // Early exit in case user told us via command line param
    if (m_running == false)
    {
        return EXIT_SUCCESS;
    }

    // Create left window
    tui::Window statusWindow{" Log ", 31, 62, 2, 5};

    // Create window for LoRa payload
    tui::Window payloadWindow{" Last payload (hex) ",
                              4,
                              60,
                              2,
                              70,
                              tui::TextPosition::CENTER,
                              tui::ColorPair::WHITE_ON_GREEN};

    // Create right window with full MQTT message
    tui::Window messageWindow{" Last full MQTT message ", 30, 60, 7, 70};

    tui::Window infoWindow{"", 4, 62, 33, 5};
    infoWindow.display() << "q: Exit the program";

    try
    {
        // Read config file
        ConfigParser mqttSettings{statusWindow, defaultConfigPath};

        // Establish connecting to TTN server
        network::MqttClient client{mqttSettings.getMqttConfig(),
                                   statusWindow,
                                   messageWindow,
                                   payloadWindow};

        auto networkLoop = [&]() {
            while (m_running)
            {
                if (client.loop() != MOSQ_ERR_SUCCESS)
                {
                    statusWindow.printLine()
                        << "Network problem, please restart";
                    /// @todo try reconneting, don't throw
                    throw;
                }
                std::this_thread::sleep_for(100ms);
            }
        };

        std::thread networkThread{networkLoop};

        while (m_running)
        {
            switch (statusWindow.getKeyStroke())
            {
            case 'q':
                statusWindow.printLine() << "Stopping gracefully..";
                m_running = false;
                break;
            case 's':
                statusWindow.printLine()
                    << "Sending dummy downlink message to all nodes";
                /// @todo
                // client.publish();
                break;
            default:
                statusWindow.printLine() << "Warning: Unkown key pressed";
                break;
            }
        }

        if (networkThread.joinable())
        {
            networkThread.join();
        }

        std::this_thread::sleep_for(3s);
        return EXIT_SUCCESS;
    }
    catch (const std::exception& exception)
    {
        statusWindow.printLine() << "Press 'q' to exit";
        statusWindow.waitForExit();
        return EXIT_FAILURE;
    }
}

} // namespace daemon
} // namespace csf

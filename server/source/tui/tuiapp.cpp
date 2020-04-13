#include "tui/tuiapp.hpp"
#include "cosyfird/configparser.hpp"
#include "network/mqttclient.hpp"
#include "tui/status_stream.hpp"
#include "tui/window.hpp"
#include "version.h"

#include <chrono>
#include <exception>
#include <getopt.h>
#include <optional>
#include <thread>

namespace csf
{
namespace tui
{
App::App()
    : NCursesApplication(true)
{
}

int App::titlesize() const
{
    return 1;
}

void App::title()
{
    using ColorPairType = std::underlying_type<ColorPair>::type;

    titleWindow = new NCursesWindow(1, 21, 0, 57);
    titleWindow->immedok(true);
    titleWindow->color_set(
        static_cast<ColorPairType>(ColorPair::BLACK_ON_YELLOW));

    std::string title{" cosyfird v"};
    title.append(versionString);
    title.append(" ");
    titleWindow->addstr(title.c_str());
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

void App::init(bool enableColors)
{
    using ColorPairType = std::underlying_type<ColorPair>::type;

    if (enableColors)
    {
        NCursesWindow::useColors();

        init_pair(static_cast<ColorPairType>(ColorPair::BLACK_ON_WHITE),
                  COLOR_BLACK,
                  COLOR_WHITE);
        init_pair(static_cast<ColorPairType>(ColorPair::WHITE_ON_BLACK),
                  COLOR_WHITE,
                  COLOR_BLACK);
        init_pair(static_cast<ColorPairType>(ColorPair::BLACK_ON_YELLOW),
                  COLOR_BLACK,
                  COLOR_YELLOW);
        init_pair(static_cast<ColorPairType>(ColorPair::WHITE_ON_GREEN),
                  COLOR_WHITE,
                  COLOR_GREEN);
    }

    // Make cursor invisible
    curs_set(0);
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
    tui::Window statusWindow{" Log ", 30, 62, 3, 5};

    // Create window for LoRa payload
    tui::Window payloadWindow{" Last payload (hex) ",
                              6,
                              60,
                              3,
                              70,
                              tui::TextPosition::CENTER,
                              ColorPair::WHITE_ON_GREEN};

    // Create right window with full MQTT message
    tui::Window messageWindow{" Last full MQTT message ", 22, 60, 11, 70};

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
                    throw;
                }
                std::this_thread::sleep_for(100ms);
            }
        };

        std::thread networkThread{networkLoop};

        statusWindow.waitForExit();

        m_running = false;
        if (networkThread.joinable())
        {
            networkThread.join();
        }

        statusWindow.printLine() << "Stopping gracefully..";
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

} // namespace tui
} // namespace csf

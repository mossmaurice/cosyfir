#include "cosyfird/configparser.hpp"
#include "tui/window.hpp"

#include <filesystem>
#include <yaml-cpp/yaml.h>

namespace csf
{
ConfigParser::ConfigParser(tui::Window& window, std::string yamlFile)
{
    window.printLine() << "Reading " << yamlFile << "..";

    try
    {
        m_config = YAML::LoadFile(yamlFile);
    }
    catch (const YAML::ParserException& exception)
    {
        window.printLine() << "YAML file could not be parsed, check syntax";
    }
    catch (const YAML::BadFile& exception)
    {
        window.printLine() << "Could not open "
                           << std::filesystem::current_path() << "/" << yamlFile;
        throw;
    }
}

const MqttConfig ConfigParser::getMqttConfig() const
{
    return MqttConfig{
        getHostAddress(), getPort(), getClientId(), getPassword()};
}

const std::string ConfigParser::getHostAddress() const
{
    return m_config["MqttConfig"]["HostAddress"].as<std::string>();
}

const uint16_t ConfigParser::getPort() const
{
    return m_config["MqttConfig"]["Port"].as<uint16_t>();
}

const std::string ConfigParser::getClientId() const
{
    return m_config["MqttConfig"]["ClientId"].as<std::string>();
}

const std::string ConfigParser::getPassword() const
{
    return m_config["MqttConfig"]["Password"].as<std::string>();
}
} // namespace csf

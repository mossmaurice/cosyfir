#include "cosyfir-sub/configparser.hpp"

#include <yaml-cpp/yaml.h>

ConfigParser::ConfigParser(std::string yamlFile)
{
    m_config = YAML::LoadFile(yamlFile);
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
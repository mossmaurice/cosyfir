#pragma once

#include <string>
#include <yaml-cpp/yaml.h>
namespace csf
{
namespace tui
{
class Window;
}
/// @todo
struct MqttConfig
{
    std::string hostAddress;
    uint16_t port;
    std::string clientId;
    std::string password;
};
class ConfigParser
{
  public:
    ConfigParser(tui::Window& window, std::string yamlFile);

    virtual ~ConfigParser() = default;

    ConfigParser(const ConfigParser&) = delete;
    ConfigParser(ConfigParser&&) = delete;
    ConfigParser& operator=(const ConfigParser&) = delete;
    ConfigParser& operator=(ConfigParser&&) = delete;

    const MqttConfig getMqttConfig() const;

    // private: @todo
    const std::string getHostAddress() const;
    const uint16_t getPort() const;
    const std::string getClientId() const;
    const std::string getPassword() const;

  private:
    YAML::Node m_config;
};

} // namespace csf

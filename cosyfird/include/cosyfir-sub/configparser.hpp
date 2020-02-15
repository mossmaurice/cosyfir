#pragma once

#include <string>
#include <yaml-cpp/yaml.h>

class ConfigParser
{
  public:
    ConfigParser(std::string yamlFile);

    virtual ~ConfigParser() = default;

    ConfigParser(const ConfigParser &) = delete;
    ConfigParser(ConfigParser &&) = delete;
    ConfigParser &operator=(const ConfigParser &) = delete;
    ConfigParser &operator=(ConfigParser &&) = delete;

    const std::string getHostAddress() const;
    const uint16_t getPort() const;
    const std::string getClientId() const;
    const std::string getPassword() const;

  private:
    YAML::Node m_config;
};
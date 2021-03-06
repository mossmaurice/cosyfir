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

#include "cosyfird/configparser.hpp"
#include "tui/status_stream.hpp"
#include "tui/window.hpp"

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
        window.printLine() << "Could not open " << yamlFile;
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

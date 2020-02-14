#include "cosyfir-sub/configparser.hpp"

#include <yaml.h>

ConfigParser::ConfigParser()
{
    YAML::Node config = YAML::LoadFile("config.yaml");
}

#pragma once

class ConfigParser
{
    ConfigParser();

    ~ConfigParser();

    ConfigParser(const ConfigParser &) = delete;
    ConfigParser(ConfigParser &&) = delete;
    ConfigParser &operator=(const ConfigParser &) = delete;
    ConfigParser &operator=(ConfigParser &&) = delete;
};
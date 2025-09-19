#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <map>

using Config = std::map<std::string, std::map<std::string, std::string>>;

Config LoadSettings(const std::string& filename);
void SetSettings(Config& config, const std::string& filename);

#endif CONFIG_H
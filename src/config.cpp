#include "config.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>

std::ofstream configFile;

using Config = std::map<std::string, std::map<std::string, std::string>>;

Config LoadSettings(const std::string& filename) {
	Config config;
	std::ifstream file(RESOURCES_PATH "configs/" + filename);
	std::string line, currentSection;

	if (!file.is_open()) {
		std::cerr << "Could not open Config!" << std::endl;
		return config;
	}

	while (std::getline(file, line)) {
		line.erase(0, line.find_first_not_of(" \t"));
		line.erase(line.find_last_not_of(" \t") + 1);

		if (line.empty() || line[0] == ';' || line[0] == '#') {
			continue;
		}

		// Sektionen [blablabla]
		if (line[0] == '[' && line.back() == ']') {
			currentSection = line.substr(1, line.size() - 2);
			config[currentSection];
			continue;
		}

		if (!currentSection.empty()) {
			size_t delimiterPos = line.find("=");
			if (delimiterPos != std::string::npos) {
				std::string key = line.substr(0, delimiterPos);
				std::string value = line.substr(delimiterPos + 1);
				key.erase(0, key.find_first_not_of(" \t"));
				key.erase(key.find_last_not_of(" \t") + 1);
				value.erase(0, value.find_first_not_of(" \t"));
				value.erase(value.find_last_not_of(" \t") + 1);
				config[currentSection][key] = value;
			}
		}
	}

	file.close();
	return config;
}

void SetSettings(Config& config, const std::string& filename) {
	std::ofstream file(RESOURCES_PATH "configs/" + filename);

	if (!file.is_open()) {
		std::cerr << "Could not open Config!" << std::endl;
		return;
	}

	for (const auto& section : config) {
		file << "[" << section.first << "]\n";
		for (const auto& keyValue : section.second) {
			file << keyValue.first << "=" << keyValue.second << "\n";
		}
		file << "\n";
	}
	file.close();
}
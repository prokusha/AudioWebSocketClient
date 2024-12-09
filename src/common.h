#pragma once

#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>

#include "audioManager.h"
#include "clientManager.h"

using json = nlohmann::json;

void runPythonScript(const std::string& scriptPath, const std::string& args);

bool isYouTubeLink(const std::string& url);

bool isYandexMusicLink(const std::string& url);

json getAudioInfo(const std::string& payload);

bool fileExist(const std::string& fileName);

void removeFile(const std::string& fileName);
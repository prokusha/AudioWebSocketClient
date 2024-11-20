#pragma once

#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>

#include "audioManager.h"
#include "clientManager.h"

using json = nlohmann::json;

enum class payloadSignal {
    PLAY,
    SYNC,
    NEXT,
    URL
};

std::unordered_map<std::string, payloadSignal> const tableSignals = { 
    {"play", payloadSignal::PLAY}, 
    {"sync", payloadSignal::SYNC}, 
    {"next", payloadSignal::NEXT},
    {"url", payloadSignal::URL} };

void runPythonScript(const std::string& scriptPath, const std::string& args);

bool isYouTubeLink(const std::string& url);

bool isYandexMusicLink(const std::string& url);

void handleUrl(const std::string& url, AudioManager& audioManager, ClientManager& clientManager, server& server);

json getAudioInfo(const std::string& payload);

bool fileExist(const std::string& fileName);

void removeFile(const std::string& fileName);

bool waitFile(const std::string& fileName);
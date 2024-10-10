#pragma once

#include <regex>
#include <iostream>
#include <unordered_map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class payloadSignal {
    PLAY,
    SYNC,
    NEXT
};

std::unordered_map<std::string, payloadSignal> const tableSignals = { 
    {"play", payloadSignal::PLAY}, 
    {"sync", payloadSignal::SYNC}, 
    {"next", payloadSignal::NEXT} };

void runPythonScript(const std::string& scriptPath, const std::string& args);

bool isYouTubeLink(const std::string& url);

bool isYandexMusicLink(const std::string& url);

json getAudioInfo(const std::string& payload);
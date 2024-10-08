#pragma once

#include <regex>
#include <iostream>

void runPythonScript(const std::string& scriptPath, const std::string& args);

bool isYouTubeLink(const std::string& url);

bool isYandexMusicLink(const std::string& url);

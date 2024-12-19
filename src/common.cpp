#include "common.h"
#include "audioManager.h"

#include <chrono>
#include <regex>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

void runPythonScript(const std::string& scriptPath, const std::string& args) {
    // Формируем команду для запуска Python-скрипта
    std::ostringstream command;
    command << "python3 " << scriptPath << " " << args;

    // Выполняем команду
    int result = system(command.str().c_str());

    // Проверяем результат выполнения
    if (result != 0) {
        std::cerr << "Ошибка при выполнении Python-скрипта. Код ошибки: " << result << std::endl;
    }
}

bool isYouTubeLink(const std::string& url) {
    std::regex youtubeRegex(R"(^(https?://)?(www\.)?(youtube\.com/watch\?v=|music\.youtube\.com/watch\?v=|youtu\.be/)[\w-]{11}.*$)");
    return std::regex_match(url, youtubeRegex);
}

bool isYandexMusicLink(const std::string& url) {
    std::regex yandexMusicRegex(R"(^(https?://)?(music\.yandex\.ru/album/\d+/track/\d+|music\.yandex\.ru/track/\d+|music\.yandex\.ru/artist/\d+))");
    return std::regex_match(url, yandexMusicRegex);
}

json getAudioInfo(bool thisId, const std::string& text, bool getLink) {
    std::string fileName = "data.json";
    
    removeFile(fileName);

    std::string script = "scripts/parser_ya-music.py";
    std::string args = "";

    if (getLink) {
        args = "-t link " + text;
    } else if (thisId) {
        args = "-t id " + text;
    } else if (isYandexMusicLink(text)) {
        args = "-t url " + text;
    } else {
        std::cout << "Тип ссылки не поддерживается" << std::endl;
        return {};
    }
    
    std::cout << "Запускаем: " << script << std::endl;
    runPythonScript(script, args);

    std::ifstream ifs(fileName);
    json jf;
    try {
        jf = json::parse(ifs);
    } catch (json::parse_error& e) {
        std::cout << "Ошибка парсинга JSON: " << e.what() << std::endl;
        return {};
    }
    std::cout << jf << std::endl;
    ifs.close();
    removeFile(fileName);

    return jf;
}

bool fileExist(const std::string &fileName) {
    std::ifstream file(fileName);
    return file.good();
}

void removeFile(const std::string& fileName) {
    if (fileExist(fileName)) {
        std::remove(fileName.c_str());
    }
}
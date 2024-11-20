#include "common.h"

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

void handleUrl(const std::string& url, AudioManager& audioManager, ClientManager& clientManager, server& server) {
    json audio_info = getAudioInfo(url);
    if (audio_info.empty()) {
        return;
    }
    audioManager.addAudio(audio_info);
    if (!audioManager.isPlaying()) {
        clientManager.broadcast(json{
            audioManager.getCurrentAudio(), 
            audioManager.getList()
        }, server);
    } else {
        clientManager.broadcast(audioManager.getList(), server);
    }
}

json getAudioInfo(const std::string& payload) {
    std::string fileName = "data.json";
    
    removeFile(fileName);

    std::string filePath = "scripts/";
    if (isYandexMusicLink(payload)) {
        filePath += "parser_ya-music.py";
    } else {
        std::cout << "Тип ссылки не поддерживается" << std::endl;
        return {};
    }
    
    std::cout << "Запускаем: " << filePath << std::endl;
    runPythonScript(filePath, "--url " + payload);

    if (!waitFile(fileName)) {
        return {};
    }

    std::ifstream ifs(fileName);
    json jf;
    try {
        jf = json::parse(ifs);
    } catch (json::parse_error& e) {
        std::cout << "Ошибка парсинга JSON: " << e.what() << std::endl;
        return {};
    }
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

bool waitFile(const std::string& fileName) {
    bool exist = fileExist(fileName);

    for (int sec = 0; sec < 10 && !exist; sec++, exist = fileExist(fileName)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    return exist;
}
#include "common.h"

#include <regex>
#include <fstream>
#include <iostream>

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

json getAudioInfo(const std::string& payload) {
    std::string filePath = "scripts/";
    if (isYouTubeLink(payload)) {
        filePath += "parser_yt-dlp.py";
    } else if (isYandexMusicLink(payload)) {
        filePath += "parser_ya-music.py";
    } else {
        std::cout << "Тип ссылки не поддерживается" << std::endl;
        return {};
    }
    std::cout << "Запускаем: " << filePath << std::endl;
    runPythonScript(filePath, "--url " + payload);

    std::ifstream ifs("data.json");
    json jf;
    try {
        jf = json::parse(ifs);
    } catch (json::parse_error& e) {
        std::cout << "Ошибка парсинга JSON: " << e.what() << std::endl;
        return {};
    }
    ifs.close();
    std::remove("data.json");

    return jf;
}
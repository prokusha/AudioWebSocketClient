#include "common.h"

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

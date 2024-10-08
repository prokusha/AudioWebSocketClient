#include "server.h"
#include "common.h"

using namespace nlohmann;

void WebSocketServer::run() {
    std::cout << "Server listening on port 9002" << std::endl;
    m_server.run();
}

void WebSocketServer::broadcast_time(double time) {
    std::cout << time << std::endl;
    for (auto client : m_clients) {
        std::string message = std::to_string(time);
        m_server.send(client, message, websocketpp::frame::opcode::text);
    }
}

void WebSocketServer::broadcast_time_new_connect(websocketpp::connection_hdl hdl, double time) {
    std::string message = std::to_string(time);
    m_server.send(hdl, message, websocketpp::frame::opcode::text);
}

void WebSocketServer::on_open(websocketpp::connection_hdl hdl) {
    m_clients.insert(hdl);
    if (audio_url.empty()) {
        return;
    }
    m_server.send(hdl, audio_url, websocketpp::frame::opcode::text);
}

void WebSocketServer::on_close(websocketpp::connection_hdl hdl) {
    m_clients.erase(hdl);
}

void WebSocketServer::handle_play_request(websocketpp::connection_hdl hdl) {
    is_playing = true;
    start_time = 0.0;

    while (is_playing && start_time < audio_duration) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        start_time += 1.0;
    }

    is_playing = false; // Останавливаем воспроизведение после достижения длительности
}

void WebSocketServer::on_message(websocketpp::connection_hdl hdl, server::message_ptr msg) {
    std::string payload = msg->get_payload();
    if (payload == "play") {
        if (!is_playing) {
            std::thread th = std::thread(&WebSocketServer::handle_play_request, this,hdl);
            count_thread = th.native_handle();
            th.detach();
        } else {
            broadcast_time_new_connect(hdl, start_time);
        }
    } else if (payload == "sync") {
        broadcast_time(start_time);
    } else if (payload == "next") {
        broadcast_queue();
    } else {
        // Обработка полученного URL
        std::cout << "Получен URL: " << payload << std::endl;
        // Отправка URL обратно всем клиентам
        broadcast_url(payload);
    }
    broadcast_list();
}

void WebSocketServer::broadcast_url(const std::string& url) {
    std::string filePath = "scripts/";
    if (isYouTubeLink(url)) {
        filePath += "parser_yt-dlp.py";
    } else if (isYandexMusicLink(url)) {
        filePath += "parser_ya-music.py";
    } else {
        std::cout << "Тип ссылки не поддерживается" << std::endl;
        return;
    }
    std::cout << "Запускаем: " << filePath << std::endl;
    runPythonScript(filePath, "--url " + url);

    std::ifstream ifs("data.json");
    json jf;
    try {
        jf = json::parse(ifs);
    } catch (json::parse_error& e) {
        std::cout << "Ошибка парсинга JSON: " << e.what() << std::endl;
        return;
    }
    ifs.close();
    std::remove("data.json");

    m_audioOrder.addAudio(jf);

    if (!is_playing) {
        broadcast_queue();
    }
    
    // audio_url = jf["url"];
    // audio_duration = jf["duration"];
    // std::cout << jf << std::endl;
    // for (auto client : m_clients) {
    //     m_server.send(client, audio_url, websocketpp::frame::opcode::text);
    // }
}

void WebSocketServer::broadcast_queue() {
    if (m_audioOrder.isEmpty()) {
        return;
    }
    if (is_playing) {
        pthread_cancel(count_thread);
    }
    is_playing = false;

    json jf = m_audioOrder.getNext();
    audio_url = jf["url"];
    audio_duration = jf["duration"];
    for (auto client : m_clients) {
        m_server.send(client, audio_url, websocketpp::frame::opcode::text);
    }
}

void WebSocketServer::broadcast_list() {
    if (m_audioOrder.isEmpty()) {
        return;
    }
    std::string list = m_audioOrder.getList();
    for (auto client : m_clients) {
        m_server.send(client, list, websocketpp::frame::opcode::text);
    }
}

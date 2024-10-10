#pragma once

#include "audioOrder.h"
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <nlohmann/json.hpp>
#include <set>

typedef websocketpp::server<websocketpp::config::asio> server;
using namespace std::placeholders;

class WebSocketServer {
public:
    WebSocketServer() : is_playing(false), start_time(0.0), audio_duration(0) {
        m_server.set_open_handler(bind(&WebSocketServer::on_open, this, ::_1));
        m_server.set_close_handler(bind(&WebSocketServer::on_close, this, ::_1));
        m_server.set_message_handler(bind(&WebSocketServer::on_message, this, ::_1, ::_2));

        m_server.init_asio();
        m_server.set_reuse_addr(true);
        m_server.listen(9002);
        m_server.start_accept();
    }

    void run();

private:
    // TODO: Разделить все это месиво 
    server m_server;
    AudioOrder m_audioOrder;
    std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> m_clients;
    bool is_playing;
    double start_time; // Время начала воспроизведения
    double audio_duration; // Длительность аудио
    std::string audio_url;
    pthread_t count_thread;

    void broadcast_time(double time);

    void broadcast_time(websocketpp::connection_hdl hdl, double time);

    void on_open(websocketpp::connection_hdl hdl);

    void on_close(websocketpp::connection_hdl hdl);

    void handle_play_request(websocketpp::connection_hdl hdl);

    void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg);

    void broadcast_url(const std::string& url);

    void broadcast_queue();
    
    void broadcast_list();

    void broadcast_list(websocketpp::connection_hdl hdl);
};

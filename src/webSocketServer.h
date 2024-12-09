#pragma once

#include "clientManager.h"
#include "audioManager.h"
#include "signalHandler.h"

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;
using namespace std::placeholders;

class WebSocketServer {
public:
    WebSocketServer() {
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
    server m_server;
    ClientManager m_clientManager;
    AudioManager m_audioManager;

    std::unordered_map<payloadSignal, std::function<void(websocketpp::connection_hdl)>> handlers;

    void on_open(websocketpp::connection_hdl hdl);
    void on_close(websocketpp::connection_hdl hdl);
    void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg);
};
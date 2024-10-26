#pragma once

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <vector>
#include <set>

typedef websocketpp::server<websocketpp::config::asio> server;

class ClientManager {
public:
    void addClient(websocketpp::connection_hdl hdl);
    void removeClient(websocketpp::connection_hdl hdl);
    void broadcast(websocketpp::connection_hdl hdl, const std::string& message, server& m_server);
    void broadcast(websocketpp::connection_hdl hdl, const std::vector<std::string>& messages, server& m_server);
    void broadcast(const std::string& message, server& m_server);
    void broadcast(const std::vector<std::string>& messages, server& m_server);

private:
    std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> m_clients;

    void sendMessage(websocketpp::connection_hdl hdl, const std::string& message, server& m_server);
};

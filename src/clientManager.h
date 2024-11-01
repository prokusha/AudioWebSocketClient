#pragma once

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <nlohmann/json.hpp>
#include <vector>
#include <set>

typedef websocketpp::server<websocketpp::config::asio> server;
using json = nlohmann::json;

class ClientManager {
public:
    void addClient(websocketpp::connection_hdl hdl);
    void removeClient(websocketpp::connection_hdl hdl);
    void broadcast(websocketpp::connection_hdl hdl, const json& message, server& m_server);
    void broadcast(websocketpp::connection_hdl hdl, const std::vector<json>& messages, server& m_server);
    void broadcast(const json& message, server& m_server);
    void broadcast(const std::vector<json>& messages, server& m_server);

private:
    std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> m_clients;

    void sendMessage(websocketpp::connection_hdl hdl, const json& message, server& m_server);
};

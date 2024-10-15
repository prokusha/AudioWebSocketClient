#include "clientManager.h"

#include <websocketpp/frame.hpp>

void ClientManager::addClient(websocketpp::connection_hdl hdl) {
    m_clients.insert(hdl);
}

void ClientManager::removeClient(websocketpp::connection_hdl hdl) {
    m_clients.erase(hdl);
}

void ClientManager::broadcast(websocketpp::connection_hdl hdl, const std::string& message, server& m_server) {
    m_server.send(hdl, message, websocketpp::frame::opcode::text);
}

void ClientManager::broadcast(websocketpp::connection_hdl hdl, const std::vector<std::string>& messages, server& m_server) {
    for (const std::string& message : messages) {
        broadcast(hdl, message, m_server);
    }
}

void ClientManager::broadcast(const std::string& message, server& m_server) {
    for (auto client : m_clients) {
        broadcast(client, message, m_server);
    }
}

void ClientManager::broadcast(const std::vector<std::string>& messages, server& m_server) {
    for (auto client : m_clients) {
        broadcast(client, messages, m_server);
    }
}


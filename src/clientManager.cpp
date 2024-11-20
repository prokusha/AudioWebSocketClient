#include "clientManager.h"

void ClientManager::addClient(websocketpp::connection_hdl hdl) {
    m_clients.insert(hdl);
}

void ClientManager::removeClient(websocketpp::connection_hdl hdl) {
    m_clients.erase(hdl);
}

void ClientManager::broadcast(websocketpp::connection_hdl hdl, const json& message, server& m_server) {
    sendMessage(hdl, message, m_server);
}

void ClientManager::broadcast(const json& message, server& m_server) {
    std::for_each(std::execution::par, m_clients.begin(), m_clients.end(), 
    [this, &message, &m_server](auto& hdl){
        sendMessage(hdl, message, m_server);
    });
}

void ClientManager::sendMessage(websocketpp::connection_hdl hdl, const json& message, server& m_server) {
    std::cout << message << std::endl;
    websocketpp::lib::error_code ec;
    m_server.send(hdl, message.dump(), websocketpp::frame::opcode::text, ec);
    if (ec) {
        std::cout << "Error sending massages: " << ec.message() << std::endl;
    }
}


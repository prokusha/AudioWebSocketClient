#include "webSocketServer.h"
#include "common.h"

void WebSocketServer::run() {
    std::cout << "Server listening on port 9002" << std::endl;
    m_server.run();
}

void WebSocketServer::on_open(websocketpp::connection_hdl hdl) {
    m_clientManager.addClient(hdl);
    if (m_audioManager.isPlaying()) {
        m_clientManager.broadcast(hdl, std::vector<json>{
            m_audioManager.getCurrentAudio(), 
            m_audioManager.getList()
        }, m_server);
    }
}

void WebSocketServer::on_close(websocketpp::connection_hdl hdl) {
    m_clientManager.removeClient(hdl);
}

void WebSocketServer::on_message(websocketpp::connection_hdl hdl, server::message_ptr msg) {
    std::string payload = msg->get_payload();
    json jsparse = json::parse(payload);
    if (tableSignals.find(jsparse["type"]) != tableSignals.end()) {
        switch (tableSignals.at(jsparse["type"])) {
            case payloadSignal::PLAY:
                m_audioManager.play();
                m_clientManager.broadcast(hdl, m_audioManager.getCurrentTime(), m_server);
                break;
            case payloadSignal::SYNC:
                m_clientManager.broadcast(m_audioManager.getCurrentTime(), m_server);
                break;
            case payloadSignal::NEXT:
                if (m_audioManager.playNext()) {
                    m_clientManager.broadcast(std::vector<json>{
                        m_audioManager.getCurrentAudio(), 
                        m_audioManager.getList()
                    }, m_server);
                }
                break;
            case payloadSignal::URL:
                json audio_info = getAudioInfo(jsparse["url"]);
                if (audio_info.empty()) {
                    return;
                }
                m_audioManager.addAudio(audio_info);
                if (!m_audioManager.isPlaying()) {
                    m_clientManager.broadcast(std::vector<json>{
                        m_audioManager.getCurrentAudio(), 
                        m_audioManager.getList()
                    }, m_server);
                } else {
                    m_clientManager.broadcast(m_audioManager.getList(), m_server);
                }
                break;
        }
    }
}
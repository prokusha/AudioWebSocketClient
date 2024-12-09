#pragma once

#include "clientManager.h"
#include "audioManager.h"

enum class payloadSignal {
    PLAY,
    SYNC,
    NEXT,
    ENDED,
    URL,
    SEARCH
};

std::unordered_map<std::string, payloadSignal> const tableSignals = { 
    {"play", payloadSignal::PLAY}, 
    {"sync", payloadSignal::SYNC}, 
    {"next", payloadSignal::NEXT},
    {"ended", payloadSignal::ENDED},
    {"url", payloadSignal::URL},
    {"search", payloadSignal::SEARCH}};

void signalPlay(websocketpp::connection_hdl hdl, AudioManager& audioManager, ClientManager& clientManager, server& server);
void signalSync(AudioManager& audioManager, ClientManager& clientManager, server& server);
void signalNext(AudioManager& audioManager, ClientManager& clientManager, server& server);
void signalEnded(AudioManager& audioManager, ClientManager& clientManager, server& server);
void signalUrl(const std::string& url, AudioManager& audioManager, ClientManager& clientManager, server& server);
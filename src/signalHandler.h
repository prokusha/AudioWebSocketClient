#pragma once

#include "clientManager.h"
#include "audioManager.h"

enum class payloadSignal {
    PLAY,
    SYNC,
    NEXT,
    ENDED,
    ADD,
    SEARCH
};

std::unordered_map<std::string, payloadSignal> const tableSignals = { 
    {"play", payloadSignal::PLAY}, 
    {"sync", payloadSignal::SYNC}, 
    {"next", payloadSignal::NEXT},
    {"ended", payloadSignal::ENDED},
    {"add", payloadSignal::ADD},
    {"search", payloadSignal::SEARCH}};

void signalPlay(websocketpp::connection_hdl hdl, AudioManager& audioManager, ClientManager& clientManager, server& server);
void signalSync(AudioManager& audioManager, ClientManager& clientManager, server& server);
void signalNext(AudioManager& audioManager, ClientManager& clientManager, server& server);
void signalEnded(AudioManager& audioManager, ClientManager& clientManager, server& server);
void signalAdd(bool thisId, const std::string& text, AudioManager& audioManager, ClientManager& clientManager, server& server);
void signalSearch(websocketpp::connection_hdl hdl, const std::string& text, ClientManager& clientManager, server& server);

#include "signalHandler.h"
#include "audioManager.h"
#include "common.h"

#include <thread>
#include <nlohmann/json.hpp>

using namespace nlohmann;

void signalPlay(websocketpp::connection_hdl hdl, AudioManager& audioManager, ClientManager& clientManager, server& server) {
    audioManager.play();
    clientManager.broadcast(hdl, audioManager.getCurrentTime(), server);
}

void signalSync(AudioManager& audioManager, ClientManager& clientManager, server& server) {
    clientManager.broadcast(audioManager.getCurrentTime(), server);
}

void signalNext(AudioManager& audioManager, ClientManager& clientManager, server& server) {
    if (audioManager.playNext()) {
        clientManager.broadcast(json{
        audioManager.getCurrentAudio(), 
        audioManager.getList()
        }, server);
    }
}

void signalEnded(AudioManager& audioManager, ClientManager& clientManager, server& server) {
    if (audioManager.isEnded()) {
        if (audioManager.playNext()) {
            clientManager.broadcast(json{
                audioManager.getCurrentAudio(), 
                audioManager.getList()
            }, server);
        }
    }
}

void signalUrl(const std::string& url, AudioManager& audioManager, ClientManager& clientManager, server& server) {
    json audio_info = getAudioInfo(url);
    if (audio_info.empty()) {
        return;
    }
    audioManager.addAudio(audio_info);
    if (!audioManager.isPlaying()) {
        clientManager.broadcast(json{
            audioManager.getCurrentAudio(), 
            audioManager.getList()
        }, server);
    } else {
        clientManager.broadcast(audioManager.getList(), server);
    }
}


#pragma once

#include "audioOrder.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class AudioManager {
public:
    void play();
    void stop();
    void addAudio(json audio_info);
    bool playNext();
    bool isPlaying() const;
    double getCurrentTime() const;
    std::string getCurrentAudio() const;
    std::string getList();

private:
    AudioOrder m_audioOrder;
    json current_audio;

    double audio_duration = 0.0;
    double start_time = 0.0;
    bool is_playing = false;
    pthread_t count_thread;
    
    void updateTime();
    void fillData();
};
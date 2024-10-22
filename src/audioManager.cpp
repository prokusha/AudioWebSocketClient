#include "audioManager.h"

#include <chrono>
#include <thread>

void AudioManager::play() {
    if (is_playing) {
        return;
    }
    is_playing = true;
    start_time = 0.0;

    std::thread th = std::thread(
        &AudioManager::updateTime, this
    );
    count_thread = th.native_handle();
    th.detach();
}

void AudioManager::stop() {
    pthread_cancel(count_thread);
    current_audio = nullptr;
    is_playing = false;
}

void AudioManager::addAudio(json audio_info) {
    if (current_audio.empty()) {
        current_audio = audio_info;
        fillData();
    } else {
        m_audioOrder.addAudio(audio_info);
    }
}

bool AudioManager::playNext() {
    if (m_audioOrder.isEmpty()) {
        return false;
    }
    stop();
    current_audio = m_audioOrder.getNext();
    fillData();
    play();
    return true;
}

bool AudioManager::isPlaying() const {
    return is_playing;
}

double AudioManager::getCurrentTime() const {
    return start_time;
}

std::string AudioManager::getCurrentAudio() const {
    if (current_audio.empty()) {
        return "";
    }
    return current_audio["url"];
}

std::string AudioManager::getList() {
    return m_audioOrder.getList();
}

void AudioManager::updateTime() {
    while (start_time < audio_duration) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(300)
        );
        start_time += 0.3;
    }

    stop();
}

void AudioManager::fillData() {
    audio_duration = current_audio["duration"];
}
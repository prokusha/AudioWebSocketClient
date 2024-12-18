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
    audio_duration = 0.0;
}

void AudioManager::addAudio(bool thisId, const std::string& text) {
    if (!m_audioOrder.addAudio(thisId, text)) {
        return;
    }
    if (current_audio.empty()) {
        current_audio = m_audioOrder.getNext();
        fillData();
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

bool AudioManager::isEnded() const {
    return audio_duration - start_time <= 1.5f;
}

json AudioManager::getCurrentTime() const {
    return {{"type", "timestamp"}, {"timestamp", start_time}};
}

json AudioManager::getCurrentAudio() const {
    json js;
    js["type"] = "play";
    js["title"] = "";
    js["author"] = "";
    js["url"] = "";
    js["cover"] = "";
    if (!current_audio.empty()) {
        js["title"] = current_audio["title"];
        js["author"] = current_audio["author"];
        js["url"] = current_audio["url"];
        js["cover"] = current_audio["cover"];
    }
    return js;
}

json AudioManager::getList() {
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
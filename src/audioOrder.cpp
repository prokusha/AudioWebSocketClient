#include "audioOrder.h"
#include "common.h"

bool AudioOrder::addAudio(bool thisId, const std::string& text) {
    json audio_info = getAudioInfo(thisId, text, m_audioOrder.size() < MAX_AUDIO);
    if (audio_info.empty()) {
        return false;
    }
    m_audioOrder.push_back(audio_info);
    list_need_update = true;
    return true;
}

bool AudioOrder::isEmpty() {
    return m_audioOrder.empty();
}

json AudioOrder::getNext() {
    json tmp = m_audioOrder.front();
    m_audioOrder.pop_front();
    if (m_audioOrder.size() >= MAX_AUDIO) {
        std::string track_id = m_audioOrder[2]["track_id"];
        std::string album_id = m_audioOrder[2]["album_id"];
        m_audioOrder[2] = getAudioInfo(true, track_id + ':' + album_id, true);
    }
    list_need_update = true;
    return tmp;
}

json AudioOrder::getList() {
    if (list_need_update) {
        if (list["items"] != nullptr)
        {
            list["items"].clear();
        }
        for (json& jf : m_audioOrder) {
            list["items"] +=
            {
                {"author", jf["author"]},
                {"title", jf["title"]},
                {"link", jf["url"] != "none"}
            };
        }
        list_need_update = false;
    }
    return list;
}
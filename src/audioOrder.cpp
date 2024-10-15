#include "audioOrder.h"

void AudioOrder::addAudio(json audioInfo) {
    m_audioOrder.push_back(audioInfo);
    list_need_update = true;
}

bool AudioOrder::isEmpty() {
    return m_audioOrder.empty();
}

json AudioOrder::getNext() {
    json tmp = m_audioOrder.front();
    m_audioOrder.pop_front();
    list_need_update = true;
    return tmp;
}

std::string AudioOrder::getList() {
    if (list_need_update) {
        list = "\n";
        for (json& jf : m_audioOrder) {
            list += jf["author"];
            list += " - ";
            list += jf["title"];
            list += '\n';
        }
        list_need_update = false;
    }
    return list;
}
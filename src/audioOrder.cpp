#include "audioOrder.h"

void AudioOrder::addAudio(nlohmann::json audioInfo) {
    m_audioOrder.push_back(audioInfo);
}

bool AudioOrder::isEmpty() {
    return m_audioOrder.empty();
}

nlohmann::json AudioOrder::getNext() {
    nlohmann::json tmp = m_audioOrder.front();
    m_audioOrder.pop_front();
    return tmp;
}

std::string AudioOrder::getList() {
    std::string list = "\n";
    for (nlohmann::json& jf : m_audioOrder) {
        list += jf["author"];
        list += " - ";
        list += jf["title"];
        list += '\n';
    }
    return list;
}
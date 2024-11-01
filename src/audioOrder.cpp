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
                {"title", jf["title"]}
            };
        }
        list_need_update = false;
    }
    return list;
}
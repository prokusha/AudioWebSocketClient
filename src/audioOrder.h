#pragma once

#include <deque>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class AudioOrder {
public:
    AudioOrder() {
        list["type"] = "queue";
    }
    void addAudio(json audioInfo);
    bool isEmpty();
    json getNext();
    json getList();

private:
    std::deque<json> m_audioOrder;
    
    json list;
    bool list_need_update = false;
};

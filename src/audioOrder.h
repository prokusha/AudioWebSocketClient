#pragma once

#include <deque>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const static int MAX_AUDIO = 3;

class AudioOrder {
public:
    AudioOrder() {
        list["type"] = "queue";
    }
    bool addAudio(bool thisId, const std::string& text);
    bool isEmpty();
    json getNext();
    json getList();

private:
    std::deque<json> m_audioOrder;
    
    json list;
    bool list_need_update = false;
};

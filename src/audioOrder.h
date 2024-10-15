#pragma once

#include <deque>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class AudioOrder {
public:
    void addAudio(nlohmann::json audioInfo);
    bool isEmpty();
    json getNext();
    std::string getList();

private:
    std::deque<json> m_audioOrder;
    
    std::string list = "\n";
    bool list_need_update = false;
};

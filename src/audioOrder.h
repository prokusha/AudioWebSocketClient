#pragma once

#include <deque>
#include <queue>
#include <nlohmann/json.hpp>

class AudioOrder {
public:
    void addAudio(nlohmann::json audioInfo);
    bool isEmpty();
    nlohmann::json getNext();
    std::string getList();

private:
    std::deque<nlohmann::json> m_audioOrder;
};

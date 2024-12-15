#include "signalHandler.h"
#include "audioManager.h"
#include "common.h"

#include <string>
#include <thread>
#include <nlohmann/json.hpp>
#include <curl/curl.h>

using namespace nlohmann;

void signalPlay(websocketpp::connection_hdl hdl, AudioManager& audioManager, ClientManager& clientManager, server& server) {
    audioManager.play();
    clientManager.broadcast(hdl, audioManager.getCurrentTime(), server);
}

void signalSync(AudioManager& audioManager, ClientManager& clientManager, server& server) {
    clientManager.broadcast(audioManager.getCurrentTime(), server);
}

void signalNext(AudioManager& audioManager, ClientManager& clientManager, server& server) {
    if (audioManager.playNext()) {
        clientManager.broadcast(json{
        audioManager.getCurrentAudio(), 
        audioManager.getList()
        }, server);
    }
}

void signalEnded(AudioManager& audioManager, ClientManager& clientManager, server& server) {
    if (audioManager.isEnded()) {
        if (audioManager.playNext()) {
            clientManager.broadcast(json{
                audioManager.getCurrentAudio(), 
                audioManager.getList()
            }, server);
        }
    }
}

void signalAdd(bool thisId, const std::string& text, AudioManager& audioManager, ClientManager& clientManager, server& server) {
    json audio_info = getAudioInfo(thisId, text);
    if (audio_info.empty()) {
        return;
    }
    audioManager.addAudio(audio_info);
    if (!audioManager.isPlaying()) {
        clientManager.broadcast(json{
            audioManager.getCurrentAudio(), 
            audioManager.getList()
        }, server);
    } else {
        clientManager.broadcast(audioManager.getList(), server);
    }
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void signalSearch(websocketpp::connection_hdl hdl, const std::string& text, ClientManager& clientManager, server& server) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        char *encoded = curl_easy_escape(curl, text.c_str(), text.length());
        std::string result(encoded);
        curl_free(encoded);

        std::string req = "https://api.music.yandex.net:443/search?text=" + result + "&page=0&type=all&nococrrect=false";
        curl_easy_setopt(curl, CURLOPT_URL, req.c_str());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            return;
        }
        json parse = json::parse(readBuffer);
        if (!parse.contains("result") || parse["result"] == "text-is-empty") {
            return;
        }
        json jsres;
        jsres["type"] = "search";
        for (auto& p : parse["result"]["tracks"]["results"]) {
            json pp;
            int id_tr = p["id"];
            int id_alb = p["albums"][0]["id"];
            pp["id"] = std::to_string(id_tr) + ':' + std::to_string(id_alb);
            std::string arts = "";
            bool isFirst = true;
            for (auto& a : p["artists"]) {
                if (isFirst) {
                    isFirst = false;
                } else {
                    arts += ", ";
                }
                arts += a["name"];
            }
            pp["author"] = arts;
            pp["title"] = p["title"];
            jsres["results"].push_back(pp);
        }

        clientManager.broadcast(hdl, jsres, server);
        curl_easy_cleanup(curl);
    }
}


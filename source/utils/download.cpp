#include "utils/download.hpp"

#include <curl/curl.h>
#include <borealis.hpp>

namespace net {
    size_t WriteCallback(void* content, size_t size, size_t nmemb, std::string* response) {
        size_t totalSize = size * nmemb;
        response->append((char*)content, totalSize);
        return totalSize;
    }

    nlohmann::json downloadRequest(const std::string& url) {
        auto curl = curl_easy_init();

        if (!curl) 
            brls::Logger::error("Error while initializing libcurl");
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        std::string response;

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "UpcomingSwitchGames");

        auto res = curl_easy_perform(curl);

        nlohmann::json jsonResponse;

        if (res != CURLE_OK) 
            brls::Logger::error("Error while performing curl request: {}", curl_easy_strerror(res));
        else 
            jsonResponse = nlohmann::json::parse(response);
        
        curl_easy_cleanup(curl);
        return jsonResponse;
    }
}
#include "utils/download.hpp"

#include <curl/curl.h>
#include <borealis.hpp>

namespace net {
    size_t WriteCallback(void* content, size_t size, size_t nmemb, std::string* response) {
        size_t totalSize = size * nmemb;
        response->append((char*)content, totalSize);
        return totalSize;
    }

    size_t WriteCallbackImage(char* ptr, size_t size, size_t nmemb, void* userdata)
    {
        std::vector<unsigned char>* buffer = static_cast<std::vector<unsigned char>*>(userdata);
        size_t total_size = size * nmemb;
        buffer->insert(buffer->end(), ptr, ptr + total_size);
        return total_size;
    }

    nlohmann::json downloadRequest(const std::string& url) {
        curl_global_init(CURL_GLOBAL_ALL);
        auto curl = curl_easy_init();

        brls::Logger::debug("Requesting {}", url);
        if (!curl) 
            brls::Logger::error("Error while initializing libcurl");
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        std::string response;

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, false);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "UpcomingSwitchGames");

        auto res = curl_easy_perform(curl);

        nlohmann::json jsonResponse;

        if (res != CURLE_OK) 
            brls::Logger::error("Error while performing curl request: {}", curl_easy_strerror(res));
        else 
            jsonResponse = nlohmann::json::parse(response);
        
        curl_easy_cleanup(curl);

        curl_global_cleanup();
        return jsonResponse;
    }


    void downloadImage(const std::string& url, std::vector<unsigned char>& buffer) {
        curl_global_init(CURL_GLOBAL_ALL);
        auto curl = curl_easy_init();

        brls::Logger::debug("Downloading image {}", url);
        if (!curl)
            brls::Logger::error("Error while initializing libcurl");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallbackImage);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        //Used to bypass dns:mitm and 90dns
        struct curl_slist* slist = nullptr;
        slist = curl_slist_append(nullptr, "Host: img-eshop.cdn.nintendo.net");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);

        curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, 120000L);

        CURLcode result = curl_easy_perform(curl);
        if(result != CURLE_OK)
            brls::Logger::error(curl_easy_strerror(result));
        
        curl_easy_cleanup(curl);
        curl_global_cleanup();   
    }
}
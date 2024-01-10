#pragma once

#include <json.hpp>

//Only used in debug
static bool verbose = false;

namespace net {
    nlohmann::json downloadRequest(const std::string& url);
    void downloadImage(const std::string& url, std::vector<unsigned char>& buffer);
}
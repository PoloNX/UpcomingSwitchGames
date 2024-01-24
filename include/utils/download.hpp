#pragma once

#include <json.hpp>

namespace net {
    nlohmann::json downloadRequest(const std::string& url);
    void downloadImage(const std::string& url, std::vector<unsigned char>& buffer);
}
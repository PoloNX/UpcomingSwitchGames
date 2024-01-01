#pragma once

#include <json.hpp>

namespace net {
    nlohmann::json downloadRequest(const std::string& url);
}
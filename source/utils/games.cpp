#include "utils/games.hpp"

#include <regex>


UpcomingGames::UpcomingGames() {
    data = net::downloadRequest("https://raw.githubusercontent.com/PoloNX/upcoming-games-link/master/US.en.json");

    for (auto& game : data) {
        games.push_back(Game(game));
    }
}

Game::Game(nlohmann::json& game) {
    if (game.find("bannerUrl") != game.end() && !game["bannerUrl"].is_null()) 
        bannerUrl = game["bannerUrl"];

    if (game.find("category") != game.end() && !game["category"].is_null()) 
        category = game["category"].get<std::vector<std::string>>();

    if (game.find("description") != game.end() && !game["description"].is_null()) 
        description = game["description"];

    if (game.find("intro") != game.end() && !game["intro"].is_null()) 
        intro = game["intro"];

    if (game.find("isDemo") != game.end() && !game["isDemo"].is_null()) 
        isDemo = game["isDemo"].get<bool>();

    if (game.find("languages") != game.end() && !game["languages"].is_null()) 
        languages = game["languages"].get<std::vector<std::string>>();

    if (game.find("name") != game.end() && !game["name"].is_null()) 
        name = game["name"];
    
    if (game.find("numberOfPlayers") != game.end() && !game["numberOfPlayers"].is_null()) 
        numberOfPlayers = game["numberOfPlayers"].get<int>();

    if (game.find("publisher") != game.end() && !game["publisher"].is_null()) 
        publisher = game["publisher"];

    if (game.find("region") != game.end() && !game["region"].is_null()) 
        region = game["region"];

    if (game.find("releaseDate") != game.end() && !game["releaseDate"].is_null()) 
        releaseDate = game["releaseDate"];

    if (game.find("screenshots") != game.end() && !game["screenshots"].is_null()) 
        screenshots = game["screenshots"].get<std::vector<std::string>>();
        
    if (game.find("size") != game.end() && !game["size"].is_null()) 
        size = game["size"].get<int>();


    //Replace img-eshop.cdn.nintendo.net by the ip
    size_t pos = bannerUrl.find("://");
    if (pos != std::string::npos) {
        pos+=3;
        size_t endPos = bannerUrl.find("/", pos);
        if(endPos != std::string::npos) {
            std::string ipPart = bannerUrl.substr(pos, endPos - pos);
            bannerUrl.replace(pos, ipPart.length(), nintendoIp);
        }
    }
}

std::vector<unsigned char> Game::downloadBanner() {
    std::vector<unsigned char> buffer;
    net::downloadImage(bannerUrl, buffer);
    banner = new brls::Image();
    banner->setImageFromMem(buffer.data(), buffer.size());

    brls::Logger::debug("Banner size: {}", buffer.size());

    return buffer;
}
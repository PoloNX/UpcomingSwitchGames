#include "utils/games.hpp"
#include "utils/constants.hpp"

#include <regex>

bool ends_with(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

bool compareReleaseDate(const Game& game1, const Game& game2) {
    if(game1.getReleaseDate() == 0)
        return false;
    return game1.getReleaseDate() < game2.getReleaseDate();
}

bool compareName(const Game& game1, const Game& game2) {
    return game1.getName() < game2.getName();
}

bool compareSize(const Game& game1, const Game& game2) {
    if (game1.getSize() == 0 && game2.getSize() == 0)
        return false;
    if (game1.getSize() == 0)
        return false;
    if (game2.getSize() == 0)
        return true;
    return game1.getSize() < game2.getSize();
}

UpcomingGames::UpcomingGames() {
    data = net::downloadRequest("https://raw.githubusercontent.com/PoloNX/upcoming-games-link/master/US.en.json");

    for (auto& game : data) {
        Game g(game);
        if(g.getTitleID() == "" || ends_with(g.getTitleID(), "000"))
            games.push_back(g);
    }

    std::sort(games.begin(), games.end(), compareReleaseDate);
}


void UpcomingGames::sort(GameSort sort) {
    if (sort == GameSort::RELEASE_DATE) {
        std::sort(games.begin(), games.end(), compareReleaseDate);
    }   
    else if (sort == GameSort::NAME) {
        std::sort(games.begin(), games.end(), compareName);
    }
    else if (sort == GameSort::SIZE) {
        std::sort(games.begin(), games.end(), compareSize);
    }
}


void Game::replaceURL(std::string& url) {
    size_t pos = url.find("://");
    if (pos != std::string::npos) {
        pos += 3;
        size_t endPos = url.find("/", pos);
        if (endPos != std::string::npos) {
            std::string ipPart = url.substr(pos, endPos - pos);
            url.replace(pos, ipPart.length(), NINTENDO_IP);
        }
    }
}

Game::Game(nlohmann::json& game) {
    if (game.find("bannerUrl") != game.end() && !game["bannerUrl"].is_null()) 
        bannerUrl = game["bannerUrl"];

    if (game.find("category") != game.end() && !game["category"].is_null()) 
        category = game["category"].get<std::vector<std::string>>();

    if (game.find("description") != game.end() && !game["description"].is_null()) 
        description = game["description"];

    if (game.find("id") != game.end() && !game["id"].is_null()) 
        titleID = game["id"];

    if (game.find("iconUrl") != game.end() && !game["iconUrl"].is_null()) 
        iconUrl = game["iconUrl"];

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

    if (game.find("releaseDate") != game.end() && !game["releaseDate"].is_null()) {
        releaseDate = game["releaseDate"];
        releaseDate_str = std::to_string(releaseDate);
        if(releaseDate_str.size() == 8) {
            releaseDate_str.insert(4, "/");
            releaseDate_str.insert(7, "/");
        }
    }
    

    if (game.find("screenshots") != game.end() && !game["screenshots"].is_null()) 
        screenshots = game["screenshots"].get<std::vector<std::string>>();
        
    if (game.find("size") != game.end() && !game["size"].is_null()) 
        size = game["size"].get<int>();


    //Replace img-eshop.cdn.nintendo.net by the ip
    replaceURL(bannerUrl);
    replaceURL(iconUrl);
    for (std::string& screenshot : screenshots) {
        replaceURL(screenshot);
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

std::vector<unsigned char> Game::startDownloadIcon() {
    bool_icon = true;
    std::vector<unsigned char> buffer;
    net::downloadImage(getIconUrl(), buffer);
    //icon->setImageFromMem(buffer.data(), buffer.size());
    brls::Logger::debug("icon size: {}", buffer.size());
    return buffer;
}
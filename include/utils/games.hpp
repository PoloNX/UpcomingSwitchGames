#pragma once

#include <vector>
#include <string>
#include <borealis.hpp>

#include "utils/download.hpp"

const std::string nintendoIp = "23.214.229.233";

class Game {
public:
    Game(nlohmann::json& game);
    Game() {};

    std::vector<unsigned char> downloadBanner();

    std::string getBannerUrl() const {return bannerUrl;}
    brls::Image* getBanner() const {return banner;}
    const std::vector<std::string>& getCategory() const {return category;}
    std::string getDescription() const {return description;}
    std::string getIconUrl() const {return iconUrl;}
    std::string getTitleID() const {return titleID;}
    std::string getIntro() const { return intro;}
    bool getIsDemo() const {return isDemo;}
    const std::vector<std::string>& getLanguages() const {return languages;}
    std::string getName() const {       return name;}
    std::string getNumberOfPlayers() const {return numberOfPlayers;}
    std::string getPublisher() const {return publisher;}
    std::string getRegion() const {return region;}
    int getReleaseDate() const {return releaseDate;}
    const std::vector<std::string>& getScreenshots() const {return screenshots;}
    int getSize() const {return size;}
private:
    std::string bannerUrl;
    brls::Image* banner;

    std::vector<std::string> category;
    std::string description;
    std::string iconUrl;
    std::string titleID;
    std::string intro;
    bool isDemo;
    std::vector<std::string> languages;
    std::string name;
    std::string numberOfPlayers;
    std::string publisher;
    std::string region;
    int releaseDate;
    std::vector<std::string> screenshots;
    int size;
};

class UpcomingGames {
public:
    UpcomingGames();
    std::vector<Game> getGames() { return games; }
private:
    nlohmann::json data;
    std::vector<Game> games;
};
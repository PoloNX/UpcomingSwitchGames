#pragma once

#include <vector>
#include <string>
#include <borealis.hpp>

#include "utils/download.hpp"

enum class GameSort {
    RELEASE_DATE,
    NAME,
    SIZE
};

class Game {
public:
    Game(nlohmann::json& game);
    Game() {};

    std::vector<unsigned char> downloadBanner();

    void setIcon(brls::Image* banner) {this->icon = icon; this->bool_icon = true;}

    bool hasIcon() {return bool_icon;}

    std::vector<unsigned char> startDownloadIcon();

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
    std::string getReleaseDate_str() const {return releaseDate_str;}
    int getReleaseDate() const {return releaseDate;}
    const std::vector<std::string>& getScreenshots() const {return screenshots;}
    int getSize() const {return size;}

    nlohmann::json getJson() const {return game;}
private:
    void replaceURL(std::string& url);

    std::string bannerUrl;
    brls::Image* banner;

    brls::Image* icon;
    bool bool_icon = false;

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
    std::string releaseDate_str;
    int releaseDate;
    std::vector<std::string> screenshots;
    int size;

    //TODO : improve this and make one function which recreate the json from the game
    nlohmann::json game;
};

class UpcomingGames {
public:
    UpcomingGames();
    std::vector<Game> getGames() { return games; }
    void sort(GameSort sort);
private:
    nlohmann::json data;
    std::vector<Game> games;
};
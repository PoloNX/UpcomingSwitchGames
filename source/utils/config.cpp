#include "utils/config.hpp"
#include "utils/constants.hpp"

#include <switch.h>
#include <filesystem>
#include <fstream>
#include <vector>

bool cp(char *filein, const char *fileout) {
    FILE *exein, *exeout;
    exein = fopen(filein, "rb");
    if (exein == NULL) {
        /* handle error */
        perror("file open for reading");
        return false;
    }
    exeout = fopen(fileout, "wb");
    if (exeout == NULL) {
        /* handle error */
        perror("file open for writing");
        return false;
    }
    size_t n, m;
    unsigned char buff[8192];
    do {
        n = fread(buff, 1, sizeof buff, exein);
        if (n) m = fwrite(buff, 1, n, exeout);
        else   m = 0;
    }
    while ((n > 0) && (n == m));
    if (m) {
        perror("copy");
        return false;
    }
    if (fclose(exeout)) {
        perror("close output file");
        return false;
    }
    if (fclose(exein)) {
        perror("close input file");
        return false;
    }
    return true;
} 

namespace cfg {
    Config::Config() {
        this->loadConfig();
        this->parseConfig();
    }

    void Config::loadConfig() {
        if(!std::filesystem::exists(CONFIG_FILE)) {
            chdir("sdmc:/");
            std::filesystem::create_directories(CONFIG_PATH);
            cp("romfs:/config/config.json", CONFIG_FILE.c_str());
        }

        std::ifstream file(CONFIG_FILE); //No is open because it is created if it doesn't exist
        config = nlohmann::json::parse(file);
        file.close();
    }

    void Config::parseConfig() {
        if(config.contains("title_db_languages")) {
            title_db_languages = config["title_db_languages"];
        } else {
            title_db_languages = "US.en";
        }

        if(config.contains("favorite_games")) {
            for(auto& game : config["favorite_games"]) {
                favorite_games.push_back(Game(game));
            }
        }
        else {
            favorite_games = {};
        }

        if(config.contains("language")) {
            app_language = config["language"];
        } else {
            app_language = "en-US";
        }

    }

    void Config::addFavoriteGame(const Game& game) {
        brls::Logger::info("Adding favorite game : {}", game.getName());
        favorite_games.push_back(game);
    }

    void Config::clearFavoriteGames() {
        brls::Logger::info("Clearing favorite games");
        favorite_games.clear();
    }

    void Config::removeFavoriteGame(const Game& game) {
        brls::Logger::info("Removing favorite game : {}", game.getName());
        for(int i = 0; i < favorite_games.size(); i++) {
            if(favorite_games[i].getName() == game.getName()) {
                favorite_games.erase(favorite_games.begin() + i);
                return;
            }
        }
    }

    int Config::getTitleDBLanguagesIndex() {
        for(int i = 0; i < TITLE_DB_LANGUAGES.size(); i++) {
            if(TITLE_DB_LANGUAGES[i] == title_db_languages) {
                return i;
            }
        }
        return 0;
    }

    std::string Config::getAppLanguage() {
        return app_language;
    }

    void Config::setAppLanguage(const std::string& app_language) {
        this->app_language = app_language;
    }

    std::string Config::getTitleDBLanguages() {
        return title_db_languages;
    }

    void Config::setTitleDBLanguages(const std::string& title_db_languages) {
        this->title_db_languages = title_db_languages;
    }

    void Config::saveConfig() {
        this->config["title_db_languages"] = title_db_languages;
        this->config["favorite_games"] = {};
        for(auto& game : favorite_games) {
            this->config["favorite_games"].push_back(game.getJson());
        }
        this->config["language"] = app_language;


        std::ofstream file(CONFIG_FILE);
        file << this->config.dump(4);
        file.close();
    }
}
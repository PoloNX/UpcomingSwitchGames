#pragma once

#include <string>
#include <json.hpp>

#include "utils/games.hpp"

namespace cfg {
    class Config {
        public:
            Config();
            ~Config() {
                this->saveConfig();
            }

            void saveConfig();

            int getTitleDBLanguagesIndex();
            std::string getTitleDBLanguages();
            void setTitleDBLanguages(const std::string& title_db_languages);

            std::string getAppLanguage();
            void setAppLanguage(const std::string& app_language);

            void clearFavoriteGames();
            void removeFavoriteGame(const Game& game);
            void addFavoriteGame(const Game& game);
            std::vector<Game> getFavoriteGames() { return favorite_games; } 

        private:
            std::string title_db_languages;
            std::vector<Game> favorite_games;

            std::string app_language;

            void loadConfig();
            void parseConfig();

            nlohmann::json config;
    };
}
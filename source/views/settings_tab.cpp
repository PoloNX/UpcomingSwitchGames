#include "views/settings_tab.hpp"

#include "utils/config.hpp"
#include "utils/constants.hpp"

#include <filesystem>

using namespace brls::literals;

SettingsTab::SettingsTab(){
    this->inflateFromXMLRes("xml/tabs/settings.xml");
    cfg::Config config;

    titledb_selector->init("title-db languages", TITLE_DB_LANGUAGES, config.getTitleDBLanguagesIndex(), [](int selected){}, [](int selected) {
        cfg::Config config;
        config.setTitleDBLanguages(TITLE_DB_LANGUAGES[selected]);
    });

    //Generate the laguage list using the directory romfs:/i18n/
    std::filesystem::path i18n_path = "romfs:/i18n/";
    std::vector<std::string> languages = {"auto"};
    for (const auto& entry : std::filesystem::directory_iterator(i18n_path)) {
        if (entry.is_directory()) {
            languages.push_back(entry.path().filename().string());
        }
    }

    int selected = 0;
    std::string current_language = config.getAppLanguage();
    for (int i = 0; i < languages.size(); i++) {
        if (languages[i] == current_language) {
            selected = i;
            break;
        }
    }

    language_selector->init("app language", languages, selected, [](int selected){}, [moved_languages = std::move(languages)](int selected){
        cfg::Config config;
        config.setAppLanguage(moved_languages[selected]);
    });
}

brls::View* SettingsTab::create()
{
    return new SettingsTab();
}

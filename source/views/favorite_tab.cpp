#include "views/favorite_tab.hpp"   
#include "views/game_view.hpp"

#include "utils/config.hpp"

using namespace brls::literals;

FavoriteRecyclerCell::FavoriteRecyclerCell() {
    this->inflateFromXMLRes("xml/cells/cell.xml");
}

FavoriteRecyclerCell* FavoriteRecyclerCell::create() {
    return new FavoriteRecyclerCell();
}

// DATA SOURCE

FavoriteDataSource::FavoriteDataSource(){
    cfg::Config config;
    favorite_games = config.getFavoriteGames();
}

int FavoriteDataSource::numberOfRows(brls::RecyclerFrame* recycler, int section)
{
    return favorite_games.size();
}

int FavoriteDataSource::numberOfSections(brls::RecyclerFrame* recycler)
{
    return 1;
}

void FavoriteDataSource::refreshData() {
    cfg::Config config;
    favorite_games = config.getFavoriteGames();
}

std::string FavoriteDataSource::titleForHeader(brls::RecyclerFrame* recycler, int section) 
{
    if (section == 0)
        return "";
    return "Section #" + std::to_string(section+1);
}

brls::RecyclerCell* FavoriteDataSource::cellForRow(brls::RecyclerFrame* recycler, brls::IndexPath indexPath)
{
    FavoriteRecyclerCell* item = (FavoriteRecyclerCell*)recycler->dequeueReusableCell("Cell");
    item->label->setText(favorite_games[indexPath.row].getName());
    item->subtitle->setText(fmt::format("Release Date : {}, ", favorite_games[indexPath.row].getReleaseDate_str()));
    item->subtitle->setTextColor(nvgRGBA(100, 100, 100, 255));

    item->registerAction("app/hints/unfavorite"_i18n , brls::ControllerButton::BUTTON_X, [this, item, indexPath](brls::View* view){
        brls::Logger::info("unfavorite : {}", favorite_games[indexPath.row].getName());
        cfg::Config config;
        config.removeFavoriteGame(favorite_games[indexPath.row]);
        favorite_games = config.getFavoriteGames();
        config.saveConfig();
        return true;
    });

    return item;
}

void FavoriteDataSource::didSelectRowAt(brls::RecyclerFrame* recycler, brls::IndexPath indexPath)
{ 
    brls::Application::pushActivity(new brls::Activity(new GameView(favorite_games[indexPath.row])));    
}

FavoriteGamesTab::FavoriteGamesTab() {
    // Inflate the tab from the XML file
    this->inflateFromXMLRes("xml/tabs/recycling_list.xml");

    dataSource = new FavoriteDataSource();

    this->registerAction("app/hints/clear"_i18n, brls::ControllerButton::BUTTON_Y, [this](brls::View* view){
        cfg::Config config;

        config.clearFavoriteGames();
        
        //Manually save config because the destructor is called after the function
        config.saveConfig();

        this->dataSource->refreshData();

        this->recycler->reloadData();

        return true;
    });

    recycler->estimatedRowHeight = 70;
    recycler->registerCell("Cell", []() { return FavoriteRecyclerCell::create(); });
    recycler->setDataSource(dataSource);
}

brls::View* FavoriteGamesTab::create() {
    return new FavoriteGamesTab();
}
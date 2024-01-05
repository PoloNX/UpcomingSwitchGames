#include "views/recycling_list_tab.hpp"
#include "views/game_view.hpp"

#include "utils/download.hpp"
#include "utils/games.hpp"

#include <json.hpp>

RecyclerCell::RecyclerCell()
{
    this->inflateFromXMLRes("xml/cells/cell.xml");
}

RecyclerCell* RecyclerCell::create()
{
    return new RecyclerCell();
}


// DATA SOURCE

DataSource::DataSource(GameSort sort){
    upcomingGames = std::make_unique<UpcomingGames>();
    upcomingGames->sort(sort);
}

DataSource::DataSource() {
    upcomingGames = std::make_unique<UpcomingGames>();
}

void DataSource::setSort(GameSort sort) {
    upcomingGames->sort(sort);
}

int DataSource::numberOfSections(brls::RecyclerFrame* recycler)
{
    return 1;
}

int DataSource::numberOfRows(brls::RecyclerFrame* recycler, int section)
{
    return upcomingGames->getGames().size();
}
    
std::string DataSource::titleForHeader(brls::RecyclerFrame* recycler, int section) 
{
    if (section == 0)
        return "";
    return "Section #" + std::to_string(section+1);
}

/*void downloadIcon(int id, Game game, RecyclerCell* item) {
    brls::Logger::debug("Downloading icon from thread : {}", game.getIconUrl());
    std::vector<unsigned char> buffer = game.startDownloadIcon();

    if(buffer.size() == 0) {
        brls::Logger::debug("Error while downloading icon");
        return;
    }

    brls::sync([moved_buffer = std::move(buffer), game = std::move(game), item]() mutable {
        brls::Image* icon = new brls::Image();
        icon->setImageFromMem(moved_buffer.data(), moved_buffer.size());
        game.setIcon(icon);

        item->image->setImageFromMem(moved_buffer.data(), moved_buffer.size());
    });
}*/

brls::RecyclerCell* DataSource::cellForRow(brls::RecyclerFrame* recycler, brls::IndexPath indexPath)
{
    RecyclerCell* item = (RecyclerCell*)recycler->dequeueReusableCell("Cell");
    item->label->setText(upcomingGames->getGames()[indexPath.row].getName());
    item->subtitle->setText(fmt::format("Release Date : {}, ", upcomingGames->getGames()[indexPath.row].getReleaseDate_str()));
    item->subtitle->setTextColor(nvgRGBA(100, 100, 100, 255));

    //ICON DOWNLOAD
    //not used because of weird behavior
    /*item->getFocusEvent()->subscribe([this, item, indexPath](brls::View* view) {
        brls::Logger::info("focus : {}", games[indexPath.row].getName());
        if(!games[indexPath.row].hasIcon()) {
            brls::Logger::debug("Starting a new thread to download icon, hasIcon : {}, game : {}", games[indexPath.row].hasIcon(), games[indexPath.row].getName());
            std::future<void> qw = this->pool.push(downloadIcon, games[indexPath.row], item);
        }
    });*/

    return item;
}

void DataSource::didSelectRowAt(brls::RecyclerFrame* recycler, brls::IndexPath indexPath)
{ 
    brls::Application::pushActivity(new brls::Activity(new GameView(upcomingGames->getGames()[indexPath.row])));    
}

// RECYCLER VIEW

RecyclingListTab::RecyclingListTab()
{
    // Inflate the tab from the XML file
    this->inflateFromXMLRes("xml/tabs/recycling_list.xml");

    dataSource = new DataSource(GameSort::RELEASE_DATE);

    this->registerAction("Sort", brls::ControllerButton::BUTTON_Y, [this](brls::View* view){
        int selected = 0;
        brls::Dropdown* dropdown = new brls::Dropdown(
            "Sort", std::vector<std::string> {"Release Date", "Name", "Size"}, [&selected, this](int _selected) {
                selected = _selected;
                this->dataSource->setSort((GameSort)selected);
                this->recycler->reloadData();
            },
            selected);
        brls::Application::pushActivity(new brls::Activity(dropdown));
        return true;
    });

    //std::unique_ptr<UpcomingGames> upcomingGames = std::make_unique<UpcomingGames>();

    recycler->estimatedRowHeight = 70;
    //recycler->registerCell("Header", []() { return RecyclerHeader::create(); });
    recycler->registerCell("Cell", []() { return RecyclerCell::create(); });
    recycler->setDataSource(dataSource);
}

brls::View* RecyclingListTab::create()
{
    // Called by the XML engine to create a new RecyclingListTab

    return new RecyclingListTab();
}

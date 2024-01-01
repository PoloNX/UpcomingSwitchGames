#include "views/recycling_list_tab.hpp"
#include "utils/download.hpp"

#include <json.hpp>

std::vector<std::string> games;

RecyclerCell::RecyclerCell()
{
    this->inflateFromXMLRes("xml/cells/cell.xml");
}

RecyclerCell* RecyclerCell::create()
{
    return new RecyclerCell();
}

// DATA SOURCE

int DataSource::numberOfSections(brls::RecyclerFrame* recycler)
{
    return 1;
}

int DataSource::numberOfRows(brls::RecyclerFrame* recycler, int section)
{
    return games.size();
}
    
std::string DataSource::titleForHeader(brls::RecyclerFrame* recycler, int section) 
{
    if (section == 0)
        return "";
    return "Section #" + std::to_string(section+1);
}

brls::RecyclerCell* DataSource::cellForRow(brls::RecyclerFrame* recycler, brls::IndexPath indexPath)
{
    RecyclerCell* item = (RecyclerCell*)recycler->dequeueReusableCell("Cell");
    item->label->setText(games[indexPath.row]);
    //item->image->setImageFromRes("img/pokemon/thumbnails/" + pokemons[indexPath.row].id + ".png");
    return item;
}

void DataSource::didSelectRowAt(brls::RecyclerFrame* recycler, brls::IndexPath indexPath)
{
    brls::Logger::info("Item selected.");
    //recycler->present(new PokemonView(pokemons[indexPath.row]));
}

// RECYCLER VIEW

RecyclingListTab::RecyclingListTab()
{
    // Inflate the tab from the XML file
    this->inflateFromXMLRes("xml/tabs/recycling_list.xml");
    
    games.clear();
    games.push_back("Bulbasaur");

    //nlohmann::json data = net::downloadRequest("https://pokeapi.co/api/v2/pokemon?limit=151");

    recycler->estimatedRowHeight = 70;
    recycler->registerCell("Header", []() { return RecyclerHeader::create(); });
    recycler->registerCell("Cell", []() { return RecyclerCell::create(); });
    recycler->setDataSource(new DataSource());
}

brls::View* RecyclingListTab::create()
{
    // Called by the XML engine to create a new RecyclingListTab
    return new RecyclingListTab();
}

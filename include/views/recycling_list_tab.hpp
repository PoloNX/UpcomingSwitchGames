#pragma once

#include <borealis.hpp>
#include <ctpl_stl.h>
#include <functional>

#include "utils/games.hpp"

class RecyclerHeader
    : public brls::RecyclerHeader
{
};

class RecyclerCell
    : public brls::RecyclerCell
{
  public:
    RecyclerCell();

    BRLS_BIND(brls::Rectangle, accent, "brls/sidebar/item_accent");
    BRLS_BIND(brls::Label, label, "title");
    BRLS_BIND(brls::Label, subtitle, "subtitle");
    //BRLS_BIND(brls::Image, image, "image");

    static RecyclerCell* create();
};

class DataSource
    : public brls::RecyclerDataSource
{
  public:
    DataSource(GameSort sort);
    DataSource();
    void setSort(GameSort sort);
    int numberOfSections(brls::RecyclerFrame* recycler) override;
    int numberOfRows(brls::RecyclerFrame* recycler, int section) override;
    brls::RecyclerCell* cellForRow(brls::RecyclerFrame* recycler, brls::IndexPath index) override;
    void didSelectRowAt(brls::RecyclerFrame* recycler, brls::IndexPath indexPath) override;
    std::string titleForHeader(brls::RecyclerFrame* recycler, int section) override;
  private:
    std::unique_ptr<UpcomingGames> upcomingGames;
    //ctpl::thread_pool pool;
};

class RecyclingListTab : public brls::Box
{
  public:
    RecyclingListTab();

    static brls::View* create();

  private:
    DataSource* dataSource;
    BRLS_BIND(brls::RecyclerFrame, recycler, "recycler");
};

#pragma once

#include <borealis.hpp>

#include "utils/games.hpp"

class FavoriteRecyclerCell : public brls::RecyclerCell {
public:
    FavoriteRecyclerCell();

    BRLS_BIND(brls::Rectangle, accent, "brls/sidebar/item_accent");
    BRLS_BIND(brls::Label, label, "title");
    BRLS_BIND(brls::Label, subtitle, "subtitle");

    static FavoriteRecyclerCell* create();
};

class FavoriteDataSource
    : public brls::RecyclerDataSource
{
  public:
    FavoriteDataSource();
    int numberOfSections(brls::RecyclerFrame* recycler) override;
    int numberOfRows(brls::RecyclerFrame* recycler, int section) override;
    brls::RecyclerCell* cellForRow(brls::RecyclerFrame* recycler, brls::IndexPath index) override;
    void didSelectRowAt(brls::RecyclerFrame* recycler, brls::IndexPath indexPath) override;
    std::string titleForHeader(brls::RecyclerFrame* recycler, int section) override;

    void refreshData();
  private:
    std::vector<Game> favorite_games;
};

class FavoriteGamesTab : public brls::Box {
public:
    FavoriteGamesTab();

    static brls::View* create();
private:
    FavoriteDataSource* dataSource;
    BRLS_BIND(brls::RecyclerFrame, recycler, "recycler");
};
#pragma once

#include <borealis.hpp>

#include "utils/games.hpp"

class GameView : public brls::Box {
public:
    GameView(Game game);
    GameView() : GameView(Game()){}

    static brls::View* create();
private:
    Game game;
    BRLS_BIND(brls::Image, image, "image");
};
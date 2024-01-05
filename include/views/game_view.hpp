#pragma once

#include <borealis.hpp>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "utils/games.hpp"

class GameView : public brls::Box {
public:
    GameView(Game game);
    GameView() : GameView(Game()){}
    ~GameView();

    static brls::View* create();
private:
    Game game;
    BRLS_BIND(brls::Image, banner, "banner");
    BRLS_BIND(brls::Label, title, "title");
    BRLS_BIND(brls::Rectangle, image_overlay, "image_overlay");
    BRLS_BIND(brls::Label, description, "description");
    BRLS_BIND(brls::ScrollingFrame, scrolling, "scrolling");

    BRLS_BIND(brls::Label, tid, "tid");
    BRLS_BIND(brls::Label, publisher, "publisher");
    BRLS_BIND(brls::Label, size, "size");
    BRLS_BIND(brls::Label, release_date, "release_date");
    BRLS_BIND(brls::Label, languages, "languages");
    BRLS_BIND(brls::Label, types, "types");

    BRLS_BIND(brls::Box, screenshots, "screenshots");
    BRLS_BIND(brls::Box, screenshots1, "screenshots1");
    BRLS_BIND(brls::Box, screenshots2, "screenshots2");

    void downloadAndApplyImage(const std::string& url, const int& compteur);
    void runOnSecondThread();
    void stopThread();
    static void threadFunction(GameView* gameView);
    std::thread secondThread;
    std::mutex threadMutex;
    std::condition_variable threadCondition;
    bool stopThreadFlag;
};
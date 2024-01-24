#include "views/game_view.hpp"
#include "activity/main_activity.hpp"

#include <iomanip>

using namespace brls::literals;

std::string convertSizeToString(const int size) {
    const int kio = 1024;
    const int mio = kio * kio;
    const int gio = mio * kio;

    std::string string_size;

    if (size < kio) {
        string_size = std::to_string(size) + " octets";
    } else if (size < mio) {
        string_size = std::to_string(static_cast<double>(size) / kio) + " Ko";
    } else if (size < gio) {
        string_size = std::to_string(static_cast<double>(size) / mio) + " Mo";
    } else {
        string_size = std::to_string(static_cast<double>(size) / gio) + " Go";
    }

    return string_size;
}

GameView::GameView(Game game) : game(game)
{
    this->inflateFromXMLRes("xml/views/game.xml");

    auto dismissAction = [this](brls::View* view) {
        brls::Application::popActivity();
        return true;
    };

    this->setFocusable(false);
    scrolling->setFocusable(true);

    //this->setHideHighlightBackground(true);
    //this->setHideHighlightBorder(true);
    this->registerAction("Close", brls::ControllerButton::BUTTON_B, dismissAction);

    getAppletFrameItem()->title = game.getName();
    
    brls::Logger::debug("Game name: {}", game.getName());

    image_overlay->setColor(nvgRGBA(100, 100, 100, 0.8*255));

    if(game.getTitleID().size() > 0)
        tid->setText(fmt::format("{} : {}", "app/game/titleId"_i18n, game.getTitleID()));
    else
        tid->setText(fmt::format("{} : N/A", "app/game/titleId"_i18n));

    if(game.getPublisher().size() > 0)
        publisher->setText(fmt::format("{} : {}", "app/game/publisher"_i18n, game.getPublisher()));
    else
        publisher->setText(fmt::format("{} : N/A", "app/game/publisher"_i18n));

    //SIZE
    if(game.getSize() > 0) {
        std::string size_str = convertSizeToString(game.getSize());
        size->setText(fmt::format("{} : {}", "app/game/size"_i18n, size_str));
    }
    else
        size->setText(fmt::format("{} : N/A", "app/game/size"_i18n));

    //RELEASE DATE
    if(game.getReleaseDate_str().size() > 0)
        release_date->setText(fmt::format("{} : {}", "app/game/releaseDate"_i18n, game.getReleaseDate_str()));
    else
        release_date->setText(fmt::format("{} : N/A", "app/game/releaseDate"_i18n));
    
    //LANGUAGES
    if(game.getLanguages().size() > 0){
        std::string languages_str = fmt::format("{} : ", "app/game/languages"_i18n);
        for(auto& i : game.getLanguages()) {
            languages_str += i + ", ";
        }
        languages->setText(languages_str);
    }
    else
        languages->setText(fmt::format("{} : N/A", "app/game/languages"_i18n));
    
    //CATEGORY
    const auto& categories = game.getCategory();
    if (!categories.empty()) {
        types->setText(fmt::format("{} : {}", "app/game/category"_i18n, fmt::join(categories.begin(), categories.end(), ", ")));
    }
    else
        types->setText(fmt::format("{} : N/A", "app/game/category"_i18n));

    //TITLE
    title->setText(game.getName());

    //DESCRIPTION
    description->setText(game.getDescription());


    //SCREENSHOTS
    //screenshot->setImageFromRes("img/tiles.png");
    secondThread = std::thread(&GameView::threadFunction, this);
}

void GameView::downloadAndApplyImage(const std::string& url, const int& compteur) {

    {
        std::unique_lock<std::mutex> lock(threadMutex);
        if(stopThreadFlag)
            return;
    }

    std::vector<unsigned char> buffer;
    net::downloadImage(url, buffer);
    if(buffer.size()>0) 
        brls::Logger::debug("Screenshot size: {}", buffer.size());
    else{
        brls::Logger::error("Screenshot download failed");
        return;
    }


    //Push to UI using the main thread. other threads doesn't have ui components
    ASYNC_RETAIN
    brls::sync([ASYNC_TOKEN, moved_buffer = std::move(buffer), compteur]() mutable {
        ASYNC_RELEASE
        //BANNER 
        //brls::Logger::debug("compteur : {}", compteur);
        if (compteur == 0) {
            brls::Logger::info("Banner size: {}", moved_buffer.size());
            banner->setImageFromMem(moved_buffer.data(), moved_buffer.size());
            banner->setHeight(200);
        }

        //SCREENSHOTS
        else {
            brls::Image* image = new brls::Image();
            image->setImageFromMem(moved_buffer.data(), moved_buffer.size());
            image->setMaxWidth(1280 * 0.30);
            image->setMaxHeight(720 * 0.30);
            image->setMarginRight(10);
            image->setMarginLeft(10);
            image->setCornerRadius(10);
            image->setScalingType(brls::ImageScalingType::FILL);


            if(compteur < 4)
                screenshots1->addView(image);
            else if (compteur >= 4 && compteur < 7)
                screenshots2->addView(image);
        }
    });
}

void GameView::runOnSecondThread() {
    int compteur = 0;
    downloadAndApplyImage(this->game.getBannerUrl(), compteur);
    compteur++;
    for (auto& i : this->game.getScreenshots()) {
        if (compteur == 7)
            return;
        downloadAndApplyImage(i, compteur);
        
        compteur++;
    }
}

void GameView::stopThread() {
    {
        std::unique_lock<std::mutex> lock(threadMutex);
        stopThreadFlag = true;
    }
    threadCondition.notify_one();
}

void GameView::threadFunction(GameView* gameView)
{
    gameView->runOnSecondThread();
}

brls::View* GameView::create()
{
    return new GameView;
}

GameView::~GameView()
{
    stopThread();
    if(secondThread.joinable())
        secondThread.join();
}
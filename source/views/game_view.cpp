#include "views/game_view.hpp"
#include "activity/main_activity.hpp"

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
        tid->setText(fmt::format("Title id : {}", game.getTitleID()));
    else
        tid->setText("Title id : N/A");

    if(game.getPublisher().size() > 0)
        publisher->setText(fmt::format("Publisher : {}", game.getPublisher()));
    else
        publisher->setText("Publisher : N/A");

    //SIZE
    if(game.getSize() > 0)
        size->setText(fmt::format("Size : {}", game.getSize()));
    else
        size->setText("Size : N/A");

    //RELEASE DATE
    if(game.getReleaseDate_str().size() > 0)
        release_date->setText(fmt::format("Release date : {}", game.getReleaseDate_str()));
    else
        release_date->setText("Release date : N/A");
    
    //LANGUAGES
    if(game.getLanguages().size() > 0){
        std::string languages_str = "languages : ";
        for(auto& i : game.getLanguages()) {
            languages_str += i + ", ";
        }
        languages->setText(languages_str);
    }
    else
        languages->setText("languages : N/A");
    
    //CATEGORY
    const auto& categories = game.getCategory();
    if (!categories.empty()) {
        types->setText(fmt::format("Category : {}", fmt::join(categories.begin(), categories.end(), ", ")));
    }
    else
        types->setText("Category : N/A");

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
#include "views/game_view.hpp"

GameView::GameView(Game game) : game(game)
{
    this->inflateFromXMLRes("xml/views/game.xml");

    auto dismissAction = [this](brls::View* view) {
        this->dismiss();
        return true;
    };

    brls::Label* label = new brls::Label();
    label->setText(brls::Hint::getKeyIcon(brls::ControllerButton::BUTTON_RB) + "back");
    label->setFontSize(24);
    label->setMargins(0, 12, 0, 12);

    brls::Box* holder = new brls::Box();
    holder->addView(label);
    holder->setFocusable(true);
    holder->addGestureRecognizer(new brls::TapGestureRecognizer(holder));
    holder->registerClickAction(dismissAction);
    
    holder->registerAction("Close", brls::ControllerButton::BUTTON_RB, dismissAction);

    getAppletFrameItem()->title = game.getName();
    
    brls::Logger::debug("Game name: {}", game.getName());

    std::vector<unsigned char> buffer = game.downloadBanner();
    
    image->setImageFromMem(buffer.data(), buffer.size());

    //getAppletFrameItem()->setIconFromRes;*
    
    this->getView("close_button")->registerAction("OK", brls::BUTTON_A, dismissAction, false, false, brls::SOUND_BACK);
}

brls::View* GameView::create()
{
    return new GameView;
}
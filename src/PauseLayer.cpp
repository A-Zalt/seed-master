#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/CCMenuItemSpriteExtra.hpp>

using namespace geode::prelude;
// #include <geode.custom-keybinds/include/Keybinds.hpp>

class $modify(SMPauseLayer, PauseLayer) {
    void onModSettings(CCObject* sender) {
        geode::openSettingsPopup(Mod::get(), false);
    }
	void customSetup() {
        PauseLayer::customSetup();
        if (!Mod::get()->getSettingValue<bool>("hide-settings-button")) {
            auto winSize = CCDirector::sharedDirector()->getWinSize();
            auto sprite = CCSprite::create();
            auto circleSprite = CircleButtonSprite::createWithSprite("seed.png"_spr, 0.9, CircleBaseColor::Green, CircleBaseSize::MediumAlt);
            circleSprite->setScale(0.75f);
            sprite->setScale(0.9f);
            auto button = CCMenuItemSpriteExtra::create(circleSprite, this, menu_selector(SMPauseLayer::onModSettings));
            auto menu = getChildByID("left-button-menu");
            menu->addChild(button);
            button->setID("settings-button"_spr);
            button->setPosition({50, 50});
            menu->updateLayout();
        }
    }
};
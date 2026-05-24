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
            auto spr = CCSprite::create("seedButton.png"_spr);
            spr->setScale(0.75);
            auto button = CCMenuItemSpriteExtra::create(spr, this, menu_selector(SMPauseLayer::onModSettings));
            auto menu = CCMenu::create();
            addChild(menu);
            menu->setPosition({0, 0});
            menu->addChild(button);
            menu->setID("settings-menu"_spr);
            button->setPosition({50, 50});
        }
    }
};
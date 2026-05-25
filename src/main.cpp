#include <Geode/loader/SettingV3.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/UILayer.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

$on_mod(Loaded) {
    listenForSettingChanges<float>("label-scale", [](float value) {
        auto layer = PlayLayer::get();
        if (!layer || !layer->m_uiLayer) return;
        auto label = layer->m_uiLayer->getChildByID("seed-label"_spr);
        if (!label) return;
        static_cast<CCLabelBMFont*>(label)->setScale(value);
    });
    listenForSettingChanges<int>("label-opacity", [](int value) {
        auto layer = PlayLayer::get();
        if (!layer || !layer->m_uiLayer) return;
        auto label = layer->m_uiLayer->getChildByID("seed-label"_spr);
        if (!label) return;
        static_cast<CCLabelBMFont*>(label)->setOpacity(value);
    });
    listenForSettingChanges<bool>("hide-settings-button", [](bool value) {
        auto layer = CCScene::get()->getChildByType<PauseLayer>(0);
        if (!layer) return;
        auto menu = layer->getChildByID("left-button-menu");
        if (!menu) return;
        auto btn = menu->getChildByID("settings-button"_spr);
        if (!btn) return;
        btn->setVisible(!value);
        menu->updateLayout();
    });
}

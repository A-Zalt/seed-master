#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/CheckpointObject.hpp>
#include <Geode/utils/general.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/utils/random.hpp>
#include <time.h>

using namespace geode::prelude;
// #include <geode.custom-keybinds/include/Keybinds.hpp>

template <geode::utils::string::ConstexprString S, typename T>
T const& getSettingFast() {
    static T value = (
        listenForSettingChanges<T>(S.data(), [](T val) {
            value = std::move(val);
        }),
        getMod()->getSettingValue<T>(S.data())
    );
    return value;
}

class $modify(SMCheckpointObject, CheckpointObject) {
    struct Fields {
        uint64_t m_seed;
    };
};

class $modify(SMPlayLayer, PlayLayer) {	
    struct Fields {
        CCLabelBMFont* m_seedLabel;
        uint64_t m_startSeed;
        uint64_t m_startSeedCheckpoint;
        bool m_hasCheckpoint;
    };

	bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;
        if (m_uiLayer) {
            auto mod = Mod::get();
            auto winSize = CCDirector::sharedDirector()->getWinSize();
            auto label = CCLabelBMFont::create("seed", "chatFont.fnt");
            label->setPosition(ccp(winSize.width - 15, 15));
            label->setScale(mod->getSettingValue<float>("label-scale"));
            label->setOpacity(mod->getSettingValue<char>("label-opacity"));
            label->setAnchorPoint({1,0});
            label->setID("seed-label"_spr);
            m_uiLayer->addChild(label);
            m_fields->m_seedLabel = label;
        }
        m_fields->m_hasCheckpoint = false;
        return true;
    }
    void loadDefaultColors() {
        PlayLayer::loadDefaultColors();
        auto mod = Mod::get();
        auto seedGeneration = mod->getSettingValue<std::string>("seed-generation");
        if (mod->getSettingValue<bool>("force-seed")) {
            auto res = geode::utils::numFromString<uint64_t>(mod->getSettingValue<std::string>("seed"));
            GameToolbox::fast_srand(res.unwrapOr(0));
        } else if (seedGeneration != "Unchanged") {
            if (seedGeneration == "Vanilla") {
                struct timeval p;
                gettimeofday(&p, 0);
                int32_t seed = (int32_t)(p.tv_usec * (uint64_t)(p.tv_sec));
                GameToolbox::fast_srand((int32_t)seed);
            } else if (seedGeneration == "Vanilla (64-bit)") {
                struct timeval p;
                gettimeofday(&p, 0);
                uint64_t seed = p.tv_usec * p.tv_sec;
                GameToolbox::fast_srand(seed);
            } else if (seedGeneration == "Geode") {
                GameToolbox::fast_srand(geode::utils::random::generate<uint64_t>(0, 18446744073709551615));
            }
        }
        if (!getLastCheckpoint()) {
            m_fields->m_startSeed = GameToolbox::getfast_srand();
            m_fields->m_hasCheckpoint = false;
        } else {
            m_fields->m_hasCheckpoint = true;
        }
    }
    void postUpdate(float dt) {
        PlayLayer::postUpdate(dt);
        auto mod = Mod::get();
        if (!m_fields->m_seedLabel) return;
        std::string str = "";
        if (getSettingFast<"show-seed", bool>()) {
            str += fmt::format("Seed: {}\n", GameToolbox::getfast_srand());
        }
        if (getSettingFast<"show-starting-seed", bool>()) {
            str += fmt::format("Starting Seed: {}\n", m_fields->m_startSeed);
            if (m_fields->m_hasCheckpoint) {
                str += fmt::format("Checkpoint Seed: {}\n", m_fields->m_startSeedCheckpoint);
            }
        }
        m_fields->m_seedLabel->setString(str.c_str());
    }
    CheckpointObject* createCheckpoint() {
        auto checkpoint = PlayLayer::createCheckpoint();
        static_cast<SMCheckpointObject*>(checkpoint)->m_fields->m_seed = GameToolbox::getfast_srand();
        return checkpoint;
    }
    void loadFromCheckpoint(CheckpointObject* checkpoint) {
        PlayLayer::loadFromCheckpoint(checkpoint);
        auto mod = Mod::get();
        if (mod->getSettingValue<bool>("force-seed") && mod->getSettingValue<bool>("consistent-checkpoint-seed")) {
            GameToolbox::fast_srand(static_cast<SMCheckpointObject*>(checkpoint)->m_fields->m_seed);
            m_fields->m_startSeedCheckpoint = GameToolbox::getfast_srand();
        }
    }
};
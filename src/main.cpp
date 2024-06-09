#include "olc/olcPixelGameEngine.h"
#include "sol/sol.hpp"
#include "easing.h"
#include "objects.h"
#include "animation.h"
#include "map.h"
#include <memory>
#include <time.h>

class Game : public olc::PixelGameEngine {
private:
    sol::state lua;

    std::shared_ptr<Player> player = nullptr;
    std::shared_ptr<Drawable> wallTile = nullptr;

    std::unique_ptr<AnimationMove> animationMove = nullptr;
    std::unique_ptr<AnimationColorBlink> animationColor = nullptr;

    std::shared_ptr<Easing> moveEasingFunction = nullptr;
    std::shared_ptr<Easing> blinkEasingFunction = nullptr;

    int mapWidth = 30;
    int mapHeight = 20;

    std::unique_ptr<GameMap> map = nullptr;
    
    bool isAnimationActive = false;
    float completeTime = 0.3;

public:
    Game() {
        sAppName = "Example olcPixelEngine";
    }

    ~Game() {}

    bool OnUserCreate() override {
        lua.open_libraries(sol::lib::base);

        lua.new_usertype<MapConfig>("MapCfg",
            sol::constructors<MapConfig>(),
            "minimalRoomSize", sol::property(&MapConfig::minimalRoomSize),
            "maximalRoomSize", sol::property(&MapConfig::maximalRoomSize),
            "minimalAreaSize", sol::property(&MapConfig::minimalAreaSize),
            "width", sol::property(&MapConfig::width),
            "height", sol::property(&MapConfig::height),
            "rooms", sol::property(&MapConfig::rooms)
        );

        lua.script_file("./../scripts/cfg.lua");

        MapConfig mapConfig = lua["Map"].get<MapConfig>();

        player = std::make_shared<Player>(new olc::Sprite("./../assets/player.png"));
        player->setScale({2.0f, 2.0f});

        wallTile = std::make_shared<Drawable>(new olc::Sprite("./../assets/wall.png"), (olc::vf2d) {2.0, 2.0});

        moveEasingFunction = std::make_shared<EaseInOutExpo>();
        blinkEasingFunction = std::make_shared<EaseSin>();
        animationMove = std::make_unique<AnimationMove>(player, moveEasingFunction, (olc::vf2d) {0.0f, 0.0f}, completeTime);
        animationColor = std::make_unique<AnimationColorBlink>(player, blinkEasingFunction, olc::RED, completeTime, true);
        // map.generateMap(3);
        map = std::make_unique<GameMap>(mapConfig);
        map->generateMap(mapConfig.rooms);
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        if (animationMove->isFinished()) {
            CheckUserInput();
        }             

        animationMove->update(fElapsedTime);
        animationColor->update(fElapsedTime);

        Clear(olc::WHITE);

    // TODO криувая генерация или отрисовка
        for (int y = 0; y < mapHeight; y++) {
            for (int x = 0; x < mapWidth; x++) {
                if (map->getTile(x, y).getType() == TileType::FLOOR) {
                    DrawDecal({x*32.0f, y*32.0f}, wallTile->getDecal(), wallTile->getScale());
                }
            }
        }

        DrawDecal({0.0, 0.0}, wallTile->getDecal(), wallTile->getScale());
        DrawDecal(player->getPosition(), player->getDecal(), player->getScale(), player->getPixel());

        return true;
    }

    void CheckUserInput() {
        if (GetKey(olc::Key::LEFT).bHeld) {
            animationMove->setNewPosition((olc::vf2d){ player->getPosition().x - 32, player->getPosition().y });
            animationMove->start();
            animationColor->start();
        } else if (GetKey(olc::Key::RIGHT).bHeld) {
            animationMove->setNewPosition((olc::vf2d){ player->getPosition().x + 32, player->getPosition().y });
            animationMove->start();
            animationColor->start();
        } else if (GetKey(olc::Key::UP).bHeld) {
            animationMove->setNewPosition((olc::vf2d){ player->getPosition().x, player->getPosition().y - 32 });
            animationMove->start();
            animationColor->start();
        } else if (GetKey(olc::Key::DOWN).bHeld) {
            animationMove->setNewPosition((olc::vf2d){ player->getPosition().x, player->getPosition().y + 32 });
            animationMove->start();
            animationColor->start();
        }
    }
};

int main() {
    srand(time(0));

    sol::state lua;
    lua.open_libraries(sol::lib::base);
    lua.script("print (\"Hello from Lua!\")");

    Game game = Game();
    if (game.Construct(800, 600, 1, 1, false, true)) {
        game.Start();
    }
    return 0;
}
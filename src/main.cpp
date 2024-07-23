#include "olc/olcPixelGameEngine.h"
#include "sol/sol.hpp"
#include "easing.h"
#include "objects.h"
#include "animation.h"
#include "map.h"
#include "classBind.h"
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

    std::unique_ptr<GameMap> map = nullptr;
    
    const float completeTime = 0.3;

    float spriteScale = 0;
    float tileOffset = 0;

public:
    Game() {
        sAppName = "Example olcPixelEngine";
    }

    ~Game() {}

    bool OnUserCreate() override {
        srand(std::time(0));

        lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table);

        bindTypes(lua);

        lua.script_file("./../scripts/cfg.lua");
        lua.script_file("./../scripts/map.lua");

        spriteScale = lua["spriteScale"].get_or(2.0f);
        tileOffset = lua["spriteSizeModified"].get_or(32.0f);

        MapConfig mapConfig = lua["Map"].get<MapConfig>();

        player = std::make_shared<Player>(new olc::Sprite("./../assets/player.png"), (olc::vf2d) {0.0f, 0.0f}, (olc::vf2d) {spriteScale, spriteScale});

        wallTile = std::make_shared<Drawable>(new olc::Sprite("./../assets/wall.png"), (olc::vf2d) {spriteScale, spriteScale});

        moveEasingFunction = std::make_shared<EaseInOutExpo>();
        blinkEasingFunction = std::make_shared<EaseSin>();
        animationMove = std::make_unique<AnimationMove>(player, moveEasingFunction, (olc::vf2d) {0.0f, 0.0f}, completeTime);
        animationColor = std::make_unique<AnimationColorBlink>(player, blinkEasingFunction, olc::RED, completeTime, true);
        map = std::make_unique<GameMap>(lua);
        map->generateMap(mapConfig.rooms);
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        if (animationMove->isFinished()) {
            CheckUserInput();
        }             
        UpdateAnimations(fElapsedTime);
        Draw();

        return true;
    }

    void CheckUserInput() {
        if (GetKey(olc::Key::LEFT).bHeld) {
            animationMove->setNewPosition((olc::vf2d){ player->getPosition().x - 16 * player->getScale().x, player->getPosition().y });
            animationMove->start();
            animationColor->start();
        } else if (GetKey(olc::Key::RIGHT).bHeld) {
            animationMove->setNewPosition((olc::vf2d){ player->getPosition().x + 16 * player->getScale().x, player->getPosition().y });
            animationMove->start();
            animationColor->start();
        } else if (GetKey(olc::Key::UP).bHeld) {
            animationMove->setNewPosition((olc::vf2d){ player->getPosition().x, player->getPosition().y - 16 * player->getScale().y });
            animationMove->start();
            animationColor->start();
        } else if (GetKey(olc::Key::DOWN).bHeld) {
            animationMove->setNewPosition((olc::vf2d){ player->getPosition().x, player->getPosition().y + 16 * player->getScale().y });
            animationMove->start();
            animationColor->start();
        }
    }

    void UpdateAnimations(float fElapsedTime) {

        animationMove->update(fElapsedTime);
        animationColor->update(fElapsedTime);

    }

    void Draw() {
        Clear(olc::WHITE);

        for (int y = 0; y < map->getHeight(); y++) {
            for (int x = 0; x < map->getWidth(); x++) {
                if (map->getTile(x, y).getType() == TileType::WALL) {
                    DrawDecal({x*tileOffset, y*tileOffset}, wallTile->getDecal(), wallTile->getScale());
                }
            }
        }

        DrawDecal(player->getPosition(), player->getDecal(), player->getScale(), player->getPixel());
    }
};

int main() {

    sol::state lua;
    lua.open_libraries(sol::lib::base);
    lua.script("print (\"Hello from Lua!\")");

    Game game = Game();
    if (game.Construct(800, 600, 1, 1, false, true)) {
        game.Start();
    }
    return 0;
}
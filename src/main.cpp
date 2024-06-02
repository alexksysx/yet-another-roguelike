#include "olcPixelGameEngine.h"
#include "easing.h"
#include "objects.h"
#include "animation.h"
#include <memory>

class Game : public olc::PixelGameEngine {
private:
    std::shared_ptr<Player> player = nullptr;
    std::unique_ptr<AnimationMove> animationMove = nullptr;
    std::unique_ptr<AnimationColorBlink> animationColor = nullptr;
    std::shared_ptr<Easing> moveEasingFunction = nullptr;
    std::shared_ptr<Easing> blinkEasingFunction = nullptr;
    bool isAnimationActive = false;
    float completeTime = 0.3;
    float tmpTime = 0.0;
    float startPosition = 0.0;
    float endPosition = 0.0;

public:
    Game() {
        sAppName = "Example olcPixelEngine";
    }

    ~Game() {}

    bool OnUserCreate() override {
        player = std::make_shared<Player>(new olc::Sprite("./../assets/player.png"));
        player->setScale({2.0f, 2.0f});
        moveEasingFunction = std::make_shared<EaseInOutExpo>();
        blinkEasingFunction = std::make_shared<EaseSin>();
        animationMove = std::make_unique<AnimationMove>(player, moveEasingFunction, (olc::vf2d) {0.0f, 0.0f}, completeTime);
        animationColor = std::make_unique<AnimationColorBlink>(player, blinkEasingFunction, olc::RED, completeTime, true);
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        if (animationMove->isFinished()) {
            CheckUserInput();
        }             

        animationMove->update(fElapsedTime);
        animationColor->update(fElapsedTime);

        Clear(olc::WHITE);
        DrawDecal(player->getPosition(), player->getDecal(), player->getScale(), player->getPixel());
        return true;
    }

    void CheckUserInput() {
        if (GetKey(olc::Key::LEFT).bHeld) {
            animationMove->setNewPosition((olc::vf2d){ player->getPosition().x - 32, player->getPosition().y });
            animationMove->start();
        } else if (GetKey(olc::Key::RIGHT).bHeld) {
            animationMove->setNewPosition((olc::vf2d){ player->getPosition().x + 32, player->getPosition().y });
            animationMove->start();
            animationColor->start();
        } else if (GetKey(olc::Key::UP).bHeld) {
            animationMove->setNewPosition((olc::vf2d){ player->getPosition().x, player->getPosition().y - 32 });
            animationMove->start();
        } else if (GetKey(olc::Key::DOWN).bHeld) {
            animationMove->setNewPosition((olc::vf2d){ player->getPosition().x, player->getPosition().y + 32 });
            animationMove->start();
        }
    }
};

int main() {
    Game game = Game();
    if (game.Construct(800, 600, 1, 1, false, true)) {
        game.Start();
    }
    return 0;
}
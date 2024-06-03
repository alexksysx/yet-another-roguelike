#pragma once
#include <memory>
#include "objects.h"
#include "easing.h"
#include "olc/olcPixelGameEngine.h"

template <typename T>
class Animation {
protected:
    std::shared_ptr<T> object;
    float overallAnimationTime;
    float currentAnimationTime = 0.0f;
    bool finished;

public:
    Animation(std::shared_ptr<T> object, float fTime = 0.0, bool finished = true);
    virtual ~Animation() {}
    virtual void update(const float fElapsedTime);
    virtual void updateInternal() = 0;
    virtual void start();
    virtual bool isFinished();
};

class AnimationMove: public Animation<Object> {
private:
    std::shared_ptr<Easing> function;
    olc::vf2d startPosition;
    olc::vf2d newPosition;

public:
    virtual ~AnimationMove() = default;
    AnimationMove(std::shared_ptr<Object> object, std::shared_ptr<Easing> function, olc::vf2d position = {0.0, 0.0}, float fTime = 0.0, bool finished = true);
    virtual void updateInternal() override;
    void setNewPosition(const olc::vf2d position);
    void setNewEasingFunction(std::shared_ptr<Easing> function);
    void setAnimationTime(float fTime);
};

class AnimationColorBlink: public Animation<Drawable> {
private:
    std::shared_ptr<Easing> function;
    olc::Pixel color;
    olc::Pixel originalColor;

public:
    AnimationColorBlink(std::shared_ptr<Drawable> object, std::shared_ptr<Easing> function, olc::Pixel color, float fTime = 0.0, bool finished = true);
    virtual ~AnimationColorBlink() = default;
    virtual void updateInternal() override;
};
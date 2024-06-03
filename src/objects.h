#pragma once
#include "olcPixelGameEngine.h"
#include <memory>

class Object {
protected:
    olc::vf2d position;
public:
    Object(olc::vf2d position = {0.0f, 0.0f});
    virtual ~Object() = default;
    olc::vf2d getPosition() const;
    void setPosition(const olc::vf2d position);
    void move(const olc::vf2d moveVec);
};

class Drawable {
protected:
    olc::Decal* decal;
    olc::Sprite* sprite;
    olc::Pixel pixel;
    olc::vf2d scale;
public:
    Drawable(olc::Sprite* sprite, olc::vf2d scale = {1.0f, 1.0f}, olc::Pixel pixel = olc::WHITE);
    virtual ~Drawable();
    olc::Decal* getDecal() const;
    olc::Pixel getPixel() const;
    void setPixel(olc::Pixel pixel);
    olc::vf2d getScale();
    void setScale(olc::vf2d scale);
};

class Player: public Object, public Drawable {
public:
    Player(olc::Sprite* sprite, olc::vf2d position = {0.0f, 0.0f}, olc::vf2d scale = {1.0f, 1.0f}, olc::Pixel pixel = olc::WHITE);
    virtual ~Player() = default;
};
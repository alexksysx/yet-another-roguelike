#include "objects.h"

// Start of Object
Object::Object(olc::vf2d position) {
    this->position = position;
}

olc::vf2d Object::getPosition() const {
    return position;
}

void Object::setPosition(const olc::vf2d position) {
    this->position = position;
}

void Object::move(const olc::vf2d moveVec) {
    position += moveVec;
}

// end of Object

// start of Drawable

Drawable::Drawable(olc::Sprite* sprite, olc::vf2d scale, olc::Pixel pixel) {
    this->sprite = sprite;
    this->decal = new olc::Decal(sprite);
    this->pixel = pixel;
    this->scale = scale;
}

Drawable::~Drawable() {
    free(decal);
    free(sprite);
}

olc::Decal* Drawable::getDecal() const {
    return decal;
}

olc::Pixel Drawable::getPixel() const {
    return pixel;
}

void Drawable::setPixel(olc::Pixel pixel) {
    this->pixel = pixel;
}

olc::vf2d Drawable::getScale() {
    return this->scale;
}
void Drawable::setScale(olc::vf2d scale) {
    this->scale = scale;
}

// end of Drawable

// Start of Player
Player::Player(olc::Sprite* sprite, olc::vf2d position, olc::vf2d scale, olc::Pixel pixel): Object{position}, Drawable{sprite, scale, pixel} {
}

// end of Player
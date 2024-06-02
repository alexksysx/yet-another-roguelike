#include "animation.h"


// --- Start of Animation
template <typename T> Animation<T>::Animation(std::shared_ptr<T> object, float fTime, bool finished) {
    this->object = object;
    this->overallAnimationTime = fTime;
    this-> finished = finished;
}

template <typename T> void Animation<T>::update(const float fElapsedTime) {
    if (finished)
        return;
    currentAnimationTime += fElapsedTime / overallAnimationTime;
    updateInternal();
    if (currentAnimationTime >= 1.0) {
        finished = true;
    }
}

template <typename T> bool Animation<T>::isFinished() {
    return finished;
}

template <typename T> void Animation<T>::start() {
    currentAnimationTime = 0.0;
    finished = false;
}

// End of Animation


// Start of AnimationMove
AnimationMove::AnimationMove(std::shared_ptr<Object> object, std::shared_ptr<Easing> easingFunction, olc::vf2d position, float fTime, bool finished): Animation{object, fTime, finished} {
    this->function = easingFunction;
    this->newPosition = position;
    this->startPosition = object->getPosition();
}

void AnimationMove::updateInternal() {
    olc::vf2d newObjPos = {
        function->callRange(startPosition.x, newPosition.x, currentAnimationTime),
        function->callRange(startPosition.y, newPosition.y, currentAnimationTime)
    };
    object->setPosition(newObjPos);
}

void AnimationMove::setNewPosition(const olc::vf2d position) {
    this->newPosition = position;
    this->startPosition = object->getPosition();
}

void AnimationMove::setNewEasingFunction(std::shared_ptr<Easing> function) {
    this->function = function;
}
void AnimationMove::setAnimationTime(float fTime) {
    this->overallAnimationTime = fTime;
}
// End of AnimationMove


// start of AnimationColorBlink
AnimationColorBlink::AnimationColorBlink(std::shared_ptr<Drawable> object, std::shared_ptr<Easing> function, olc::Pixel color, float fTime, bool finished): Animation{object, fTime, finished} {
    this->originalColor = object->getPixel();
    this->color = color;
    this->function = function;
}

void AnimationColorBlink::updateInternal() {
    olc::Pixel tmpColor = olc::Pixel(
        function->callRange(originalColor.r, color.r, currentAnimationTime),
        function->callRange(originalColor.g, color.g, currentAnimationTime),
        function->callRange(originalColor.b, color.b, currentAnimationTime),
        function->callRange(originalColor.a, color.a, currentAnimationTime)
    );
    object->setPixel(tmpColor);
}

// end of AnimationColorBlink
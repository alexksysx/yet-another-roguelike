#pragma once
#define _USE_MATH_DEFINES

#include <cmath>


class Easing {
public:
    virtual ~Easing() {}
    virtual float call(const float fTime) = 0;

    float callRange(const float a, const float b, const float fTime) {
        return lerpf(a, b, call(fTime));
    }

    static float lerpf(float a, float b, float t) {
        if (t <= 0.0)
            return a;
        if (t >= 1.0)
            return b;
        return a + (b - a) * t;
    }
};

class EaseInExpo: public Easing {
    virtual float call(const float fTime) override {
        return pow(2, 10 * fTime - 10);
    }
};

class EaseInOutExpo: public Easing {
    virtual float call(const float fTime) override {
        return fTime == 0
          ? 0
          : fTime == 1
          ? 1
          : fTime < 0.5 ? pow(2, 20 * fTime - 10) / 2
          : (2 - pow(2, -20 * fTime + 10)) / 2;
        }
};

class EaseSin: public Easing {
    virtual float call(const float fTime) override {
        return sinf(fTime * M_PI);
    }
};
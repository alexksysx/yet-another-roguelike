#pragma once
#include "olcPixelGameEngine.h"
#include "objects.h"
#include <vector>
#include <list>
#include <random>

enum TileType{EMPTY = 0, FLOOR, WALL };

class Tile: public Drawable {
private:
    std::shared_ptr<Object> entity;
    TileType type;
public:
    Tile(TileType type, olc::Sprite* sprite, olc::vf2d scale = {1.0f, 1.0f}, olc::Pixel tint = olc::WHITE);
    virtual ~Tile() = default;
};

struct Rect {
    int x, y, w, h;

    Rect(int x = 0, int y = 0, int w = 0, int h = 0) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }

    int area() {
        return w * h;
    }

    inline bool operator==(const Rect& r) {
        return x == r.x && y == r.y && w == r.w && h == r.h;
    }
};

class GameMap {
private:
    int minRoomSize = 2;
    int maxRoomSize = 5;
    int minArea = 6;
    int width;
    int height;
    std::vector<Tile> tiles;

    std::list<Rect> splitArea(Rect rect);

public:
    GameMap(int width, int height);
    ~GameMap() = default;
    void generateMap(size_t rooms);
    Tile& getTile(int x, int y);
    std::vector<Tile>& getTiles();
};

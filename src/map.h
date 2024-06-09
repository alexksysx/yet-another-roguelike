#pragma once
#include "olc/olcPixelGameEngine.h"
#include "objects.h"
#include <vector>
#include <list>
#include <random>

enum class TileType{ EMPTY = 0, FLOOR, WALL, ENTER, EXIT };

class Tile {
private:
    std::shared_ptr<Object> entity;
    uint16_t distance;
    TileType type;
public:
    Tile(TileType tileType = TileType::EMPTY) {
        this->type = tileType;
    }

    void setType(TileType type) {
        this->type = type;
    }

    TileType getType() const {
        return type;
    }

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


struct MapConfig {
    int minimalRoomSize;
    int maximalRoomSize;
    int minimalAreaSize;
    int width;
    int height;
    int rooms;
};

class GameMap {
private:
    int minRoomSize = 2;
    int maxRoomSize = 4;
    int minArea = 6;
    int width;
    int height;
    std::vector<Tile> tiles;

    std::list<Rect> splitArea(Rect rect);

public:
    GameMap(int width, int height);
    GameMap(MapConfig config);
    ~GameMap() = default;
    void generateMap(size_t rooms);
    Tile& getTile(int x, int y);
    std::vector<Tile>& getTiles();
    const Tile& operator [](int i) const;
};

#pragma once
#include "olc/olcPixelGameEngine.h"
#include "objects.h"
#include <vector>
#include <list>
#include "sol/sol.hpp"

enum class TileType{ EMPTY = 0, NOT_EXIST, FLOOR, WALL, ENTER, EXIT };

class Tile {
private:
    std::shared_ptr<Object> entity;
    uint16_t distance;
    TileType type;
public:
    uint8_t x;
    uint8_t y;
    
    Tile(TileType tileType = TileType::EMPTY, uint8_t x = 0, uint8_t y = 0) {
        this->type = tileType;
        this->x = x;
        this->y = y;
    }

    void update(TileType tileType = TileType::EMPTY, uint8_t x = 0, uint8_t y = 0) {
        this->type = tileType;
        this->x = x;
        this->y = y;
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
    sol::state* lua;
    std::vector<Tile> tiles;
    Tile notExistTile = Tile(TileType::NOT_EXIST);

    std::list<Rect> splitArea(Rect rect);
    void connectPoints(olc::vi2d p1, olc::vi2d p2);
    void placeTilesLine(olc::vi2d p1, olc::vi2d p2);
public:
    GameMap(int width, int height);
    GameMap(MapConfig config);
    GameMap(sol::state& lua);
    ~GameMap() = default;
    void generateMap(size_t rooms);
    Tile& getTile(int x, int y);
    std::vector<Tile>& getTiles();
    const Tile& operator [](int i) const;
    int getWidth() const;
    int getHeight() const;
    std::vector<Tile*> getSurroundingTiles(uint8_t x, uint8_t y);
};

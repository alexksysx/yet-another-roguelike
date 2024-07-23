#include "map.h"

// Start of Map

GameMap::GameMap(int width, int height) {
    this->width = width;
    this->height = height;
    this->tiles = std::vector<Tile>(width * height, Tile(TileType::EMPTY));
}

GameMap::GameMap(MapConfig config) {
    this->width = config.width;
    this->height = config.height;
    this->minRoomSize = config.minimalRoomSize;
    this->maxRoomSize = config.maximalRoomSize;
    this->minArea = config.minimalAreaSize;
    this->tiles = std::vector<Tile>(width * height, Tile(TileType::EMPTY));
}

GameMap::GameMap(sol::state& lua) {
    this->lua = &lua;
    MapConfig config = lua["Map"].get<MapConfig>();
    this->width = config.width;
    this->height = config.height;
    this->minRoomSize = config.minimalRoomSize;
    this->maxRoomSize = config.maximalRoomSize;
    this->minArea = config.minimalAreaSize;
    this->tiles = std::vector<Tile>(width * height, Tile(TileType::EMPTY));
}

std::vector<Tile>& GameMap::getTiles() {
    return this->tiles;
}

Tile& GameMap::getTile(int x, int y) {
    if (x >= width || x < 0 || y < 0 || y >= height)
        return notExistTile;
    return tiles[x+(y*width)];
}

const Tile& GameMap::operator [](int i) const {
    return tiles[i];
}

int GameMap::getWidth() const {
    return width;
}
int GameMap::getHeight() const {
    return height;
}

void GameMap::generateMap(size_t rooms_count) {

    std::list<Rect> rooms = 
    (*lua)["generateMap"](rooms_count, this).get<std::list<Rect>>();

    int i = 0;
    for (Rect& a: rooms) {
        std::cout << "Room " << i << ", x = " << a.x << ", y = " << a.y << ", w = " << a.w << ", h = " << a.h << "\n" ;
        i++;
    }

    // build the wall
    for (Tile& tile: tiles) {
        if (tile.getType() == TileType::FLOOR) {
            std::vector<Tile*> surrounding = getSurroundingTiles(tile.x, tile.y);
            for (Tile* stile: surrounding) {
                if (stile->getType() == TileType::EMPTY)
                    stile->setType(TileType::WALL);
            }
        }
    }

}

void GameMap::placeTilesLine(olc::vi2d p1, olc::vi2d p2) {
    if (p1.x == p2.x) {
        int y1 = p1.y > p2.y ? p2.y : p1.y;
        int y2 = p1.y == y1 ? p2.y : p1.y;
        for (int y = y1; y <= y2; y++)
            tiles[p1.x + (y*width)].update(TileType::FLOOR, p1.x, y);
    } else if (p1.y == p2.y) {
        int x1 = p1.x > p2.x ? p2.x : p1.x;
        int x2 = p1.x == x1 ? p2.x : p1.x;
        for (int x = x1; x <= x2; x++)
            tiles[x + (p1.y*width)].update(TileType::FLOOR, x, p1.y);
    }
}

void GameMap::connectPointsVi2d(olc::vi2d p1, olc::vi2d p2) {
    if (rand()%2 == 0) {
        placeTilesLine({p1.x, p1.y}, {p1.x, p2.y});
        placeTilesLine({p1.x, p2.y}, {p2.x, p2.y});
    } else {
        placeTilesLine({p1.x, p1.y}, {p2.x, p1.y});
        placeTilesLine({p2.x, p1.y}, {p2.x, p2.y});
    }
}

void GameMap::connectPoints(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
    connectPointsVi2d((olc::vi2d){x1, y1}, (olc::vi2d){x2, y2});
}

std::vector<Tile*> GameMap::getSurroundingTiles(uint8_t x, uint8_t y) {
    std::vector<Tile*> stiles;
    for (int tx = x - 1; tx < x + 2; tx++) {
        for (int ty = y - 1; ty < y + 2; ty++) {
            Tile& t = getTile(tx, ty);
            if (t.getType() != TileType::NOT_EXIST && (tx != x && ty != y)) {
                t.update(t.getType(), tx, ty);
                stiles.push_back(&t);
            }
        }
    }
    return stiles;
}

void GameMap::setTile(uint8_t tileType, int x, int y) {
    TileType type = static_cast<TileType>(tileType);
    tiles[x + (y*width)].update(type, x, y);
}

// end of Map

// Start of Tile

// end of Tile
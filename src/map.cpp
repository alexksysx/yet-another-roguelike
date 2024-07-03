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

    std::list<Rect> rooms = std::list<Rect>();
    if (!lua) {
        std::list<Rect> areas;
        std::list<Rect> tmp;
        Rect firstArea = Rect(0, 0, this->width, this->height);
        areas = splitArea(firstArea);
        while (areas.size() < rooms_count) {
            tmp = areas;
            Rect* biggest = &tmp.front();
            for (Rect& r: tmp) {
                if (biggest->area() < r.area()) {
                    biggest = &r;
                }
            }
            std::list<Rect> newAreas = splitArea(*biggest);
            areas.remove(*biggest);
            // TODO check if there will be memory leak (it shouldn't)
            // free(biggest);
            areas.push_back(newAreas.front());
            if (newAreas.size() > 1) {
                areas.push_back(newAreas.back());
            } else {
                break;
            }
        }

        int i = 0;
        for (Rect& a: areas) {
            std::cout << "Area " << i << ", x = " << a.x << ", y = " << a.y << ", w = " << a.w << ", h = " << a.h << "\n" ;
            i++;
        }

        // make rooms inside areas

        for (Rect& r: areas) {
            int mod = this->maxRoomSize - this->minRoomSize;
            int roomWidth = mod == 0 ? this->minRoomSize : this->minRoomSize + rand()%(mod + 1);
            int roomHeight = mod == 0 ? this->minRoomSize : this->minRoomSize + rand()%(mod + 1);

            int roomXMod = r.w - roomWidth - 1;
            int roomX = r.x + 1 + rand()%(roomXMod);

            int roomYMod = r.h  - roomHeight - 1;
            int roomY  = r.y  +  1  + rand()%(roomYMod);

            rooms.push_back(Rect(roomX, roomY, roomWidth, roomHeight));
        }
    } else {
        std::cout << "Room generation from lua\n";
        rooms = (*lua)["generateMap"](rooms_count).get<std::list<Rect>>();
    }

    int i = 0;
    for (Rect& a: rooms) {
        std::cout << "Room " << i << ", x = " << a.x << ", y = " << a.y << ", w = " << a.w << ", h = " << a.h << "\n" ;
        i++;
    }

    // make tiles from rooms
    for (Rect& r: rooms) {
        for (int y = r.y; y < r.h + r.y; y++) {
            for (int x = r.x; x < r.w + r.x; x++) {
                tiles[x + (y*width)].update(TileType::FLOOR, x, y);
                std::cout << "coord x: " << x << ", coord y: " << y << "; " << (tiles[x+(y*width)].getType() == TileType::FLOOR ? "FLOOR": "EMPTY") << std::endl;
            }
        }
    }

    // connect rooms
    std::list<Rect>::iterator roomIter = rooms.begin();
    while (roomIter != rooms.end()) {
        Rect r1 = *roomIter;
        Rect r2 = *++roomIter;
        if (roomIter == rooms.end())
            break;
        connectPoints(
            {
                r1.x + rand()%r1.w,
                r1.y + rand()%r1.h
            },
            {
                r2.x + rand()%r2.w,
                r2.y + rand()%r2.h
            }
        );
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

std::list<Rect> GameMap::splitArea(Rect rect) {
    std::list<Rect> areas = std::list<Rect>();
    if (rect.w >= rect.h) {
        if (rect.w < this->minArea * 2) {
            areas.push_back(rect);
        } else {
            int mod = rect.w - this->minArea - this-> minArea;
            int dx = mod == 0 ? rect.x + this->minArea : rect.x + this->minArea + rand()%(mod);
            areas.push_back(Rect(rect.x, rect.y, dx - rect.x, rect.h));
            areas.push_back(Rect(dx, rect.y, rect.w - (dx - rect.x), rect.h));
        }

    } else {
        if (rect.h < this->minArea * 2) {
            areas.push_back(rect);
        } else {
            int mod = rect.h - this->minArea - this-> minArea;
            int dy = mod == 0 ? rect.y + this->minArea : rect.y + this->minArea + rand()%(mod);
            areas.push_back(Rect(rect.x, rect.y, rect.w, dy - rect.y));
            areas.push_back(Rect(rect.x, dy, rect.w, rect.h - (dy - rect.y)));
        }
    }
    return areas;
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

void GameMap::connectPoints(olc::vi2d p1, olc::vi2d p2) {
    if (rand()%2 == 0) {
        placeTilesLine({p1.x, p1.y}, {p1.x, p2.y});
        placeTilesLine({p1.x, p2.y}, {p2.x, p2.y});
    } else {
        placeTilesLine({p1.x, p1.y}, {p2.x, p1.y});
        placeTilesLine({p2.x, p1.y}, {p2.x, p2.y});
    }
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


// end of Map

// Start of Tile

// end of Tile
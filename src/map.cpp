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

std::vector<Tile>& GameMap::getTiles() {
    return this->tiles;
}

Tile& GameMap::getTile(int x, int y) {
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

    std::list<Rect> rooms = std::list<Rect>();
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

    i = 0;
    for (Rect& a: rooms) {
        std::cout << "Room " << i << ", x = " << a.x << ", y = " << a.y << ", w = " << a.w << ", h = " << a.h << "\n" ;
        i++;
    }

    // make tiles from rooms
    for (Rect& r: rooms) {
        for (int y = r.y; y <= r.h + r.y; y++) {
            for (int x = r.x; x <= r.w + r.x; x++) {
                std::cout << "coord x: " << x << ", coord y: " << y << "; " << (tiles[x+(y*width)].getType() == TileType::FLOOR ? "FLOOR": "EMPTY") << std::endl;
                tiles[x + (y*width)].setType(TileType::FLOOR);
                std::cout << "coord x: " << x << ", coord y: " << y << "; " << (tiles[x+(y*width)].getType() == TileType::FLOOR ? "FLOOR": "EMPTY") << std::endl;
            }
        }
    }

    // connect 

    // build the wall

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



// end of Map

// Start of Tile

// end of Tile
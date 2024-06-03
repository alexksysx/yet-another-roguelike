#include "map.h"

// Start of Map

GameMap::GameMap(int width, int height) {
    this->width = width;
    this->height = height;
    this->tiles = std::vector<Tile>(width * height);
}

void GameMap::generateMap(size_t rooms) {
    std::list<Rect> areas;
    std::list<Rect> tmp;
    Rect firstArea = Rect(0, 0, this->width, this->height);
    areas = splitArea(firstArea);
    while (areas.size() < rooms) {
        tmp = areas;
        Rect* biggest = &tmp.front();
        for (Rect& r: tmp) {
            if (biggest->area() < r.area()) {
                biggest = &r;
            }
        }
        std::list<Rect> newAreas = splitArea(*biggest);
        areas.remove(*biggest);
        // TODO check if there will be memory leak
        // free(biggest);
        areas.push_back(newAreas.front());
        if (newAreas.size() > 1) {
            areas.push_back(newAreas.back());
        } else {
            break;
        }
    }

    // make rooms inside areas

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
            areas.push_back(Rect(dx + 1, rect.y, rect.w - (dx - rect.x), rect.h));
        }

    } else {
        if (rect.h < this->minArea * 2) {
            areas.push_back(rect);
        } else {
            int mod = rect.h - this->minArea - this-> minArea;
            int dy = mod == 0 ? rect.y + this->minArea : rect.y + this->minArea + rand()%(mod);
            areas.push_back(Rect(rect.x, rect.y, rect.w, dy - rect.y));
            areas.push_back(Rect(rect.x, dy + 1, rect.w, rect.h - (dy - rect.y)));
        }
    }
    return areas;
}



// end of Map

// Start og Tile

// end of Tile
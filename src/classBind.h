#pragma once
#include "map.h"
#include "objects.h"
#include "sol/sol.hpp"
#include "olc/olcPixelGameEngine.h"

void bindTypes(sol::state& lua) {
    lua.new_usertype<MapConfig>("MapCfg",
        sol::constructors<MapConfig>(),
        "minimalRoomSize", &MapConfig::minimalRoomSize,
        "maximalRoomSize", &MapConfig::maximalRoomSize,
        "minimalAreaSize", &MapConfig::minimalAreaSize,
        "width", &MapConfig::width,
        "height", &MapConfig::height,
        "rooms", &MapConfig::rooms
    );

    lua.new_usertype<GameMap>("GameMap",
        "setTile", GameMap::setTile,
        "connectPoints", GameMap::connectPoints
    );

    lua.new_usertype<Rect>("Rect",
        sol::constructors<Rect(int, int, int, int)>(),
        "x", &Rect::x,
        "y", &Rect::y,
        "w", &Rect::w,
        "h", &Rect::h,
        "area", &Rect::area,
        "print", &Rect::print
    );

    lua.new_enum<TileType>("TileType", {
        {"EMPTY", TileType::EMPTY},
        {"NOT_EXIST", TileType::NOT_EXIST},
        {"FLOOR", TileType::FLOOR},
        {"WALL", TileType::WALL},
        {"ENTER", TileType::ENTER},
        {"EXIT", TileType::EXIT}
    });
}
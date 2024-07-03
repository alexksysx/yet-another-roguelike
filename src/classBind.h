#pragma once
#include "map.h"
#include "objects.h"
#include "sol/sol.hpp"

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

    lua.new_usertype<Rect>("Rect",
        sol::constructors<Rect(int, int, int, int)>(),
        "x", &Rect::x,
        "y", &Rect::y,
        "w", &Rect::w,
        "h", &Rect::h,
        "area", &Rect::area
    );
}
Map = MapCfg:new()
Map.minimalRoomSize = 5
Map.maximalRoomSize = 15
Map.minimalAreaSize = 15
Map.width = 40
Map.height = 50
Map.rooms = 11

function splitArea(rect)
    local areas = {}
    if (rect.w >= rect.h) then

        if (rect.w < Map.minimalAreaSize * 2) then
            table.insert(areas, rect)
        else
            local mod = rect.w - Map.minimalAreaSize - Map.minimalAreaSize
            local dx
            if mod == 0 then
                dx = rect.x + Map.minimalAreaSize
            else dx = rect.x + Map.minimalAreaSize + math.random(0, mod-1)
            end
            table.insert(
                areas,
                Rect.new(rect.x, rect.y, dx - rect.x, rect.h)
            )
            table.insert(
                areas,
                Rect.new(dx, rect.y, rect.w - (dx - rect.x), rect.h)
            )
        end
    else
        if (rect.h < Map.minimalAreaSize * 2) then
            table.insert(areas, rect)
        else
            local mod = rect.h - Map.minimalAreaSize - Map.minimalAreaSize
            local dy
            if mod == 0 then
                dy = rect.y + Map.minimalAreaSize
            else dy = rect.y + Map.minimalAreaSize + math.random(0, mod-1)
            end
            table.insert(
                areas,
                Rect.new(rect.x, rect.y, rect.w, dy - rect.y)
            )
            table.insert(
                areas,
                Rect.new(rect.x, dy, rect.w, rect.h - (dy - rect.y))
            )
        end
    end
    return areas
end

function generateMap(room_count)
    local firstArea = Rect.new(0, 0, Map.width, Map.height)

    local areas = splitArea(firstArea)
    while (#areas < room_count) do
        local biggestIndex = 1
        local biggestArea = areas[biggestIndex]
        for i, r in pairs(areas) do
            if (biggestArea:area() < r:area()) then
                biggestIndex = i
                biggestArea = r
            end
        end
        local newAreas = splitArea(biggestArea)
        table.remove(areas, biggestIndex)
        table.insert(areas, newAreas[1])
        if (#newAreas > 1) then
            table.insert(areas, newAreas[2])
        else
            break
        end
    end

    local rooms = {}
    for _, r in pairs(areas) do
        local maxXSize
        local maxYSize
        if (r.w - Map.maximalRoomSize < 2) then maxXSize = r.w - 2 else maxXSize = Map.maximalRoomSize end
        if (r.h - Map.maximalRoomSize < 2) then maxYSize = r.h - 2 else maxYSize = Map.maximalRoomSize end
        local modX = maxXSize - Map.minimalRoomSize
        local modY = maxYSize - Map.minimalRoomSize

        local roomWidth
        local roomHeight
        if modX == 0 then
            roomWidth = Map.minimalRoomSize
            roomHeight = Map.minimalAreaSize
        else
            roomWidth = Map.minimalRoomSize + math.random(0, modX)
            roomHeight = Map.minimalRoomSize + math.random(0, modY)
        end

        local roomModX = r.w - roomWidth - 2
        local roomX = r.x + 1 + math.random(0, roomModX)

        local roomModY = r.h - roomHeight - 2
        local roomY = r.y + 1 + math.random(0, roomModY)

        table.insert(rooms, Rect.new(roomX, roomY, roomWidth, roomHeight))
    end

    return rooms
end
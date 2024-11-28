local tiled = require("tiled")
local geo = require("geometry")

local map = {}

function map.id_at(tilemap, x, y)
	return geo.point(x // tilemap.tilewidth, y // tilemap.tilewidth)
end

function map.rect_at(tilemap, id)
	return geo.rect_at(id, tilemap.tilesize, tilemap.width, tilemap.height)
end

return map

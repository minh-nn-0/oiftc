local tiled = {}

TILED_FLIPFLAG_H = 0x80000000;
TILED_FLIPFLAG_V = 0x40000000;
TILED_FLIPFLAG_D = 0x20000000;
TILED_FLIPFLAG_MASK = TILED_FLIPFLAG_H | TILED_FLIPFLAG_V | TILED_FLIPFLAG_D;

function tiled.get_tileid_with_flipflags(id)
	local fliph = 1
	local flipv = 1

	if (id & TILED_FLIPFLAG_H) then fliph = -1 end
	if (id & TILED_FLIPFLAG_V) then flipv = -1 end

	return id & (id ~ TILED_FLIPFLAG_MASK), fliph, flipv
end

return tiled

local json = require("libs.json")
local tmap = dofile("../data/tutorial.lua")

for _,v in pairs(tmap.layers[1].layers[1].data) do
	io.write(v .. ",")
end


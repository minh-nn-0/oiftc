local geo = require("geometry")
local camera = {}

function camera.new()
	return {
		view = geo.rect.new(0,0,500,500),
		zoom = 1,
		smooth_speed = 0.06
	}
end

function camera.target(cam, target_x, target_y, dt)
	local dest_x = target_x - cam.view.width / 2 / cam.zoom - cam.view.x
	local dest_y = target_y - cam.view.height / 2 / cam.zoom - cam.view.y

	cam.view.x = cam.view.x + dest_x * cam.smooth_speed * dt
	cam.view.x = cam.view.y + dest_y * cam.smooth_speed * dt
end

function camera.zoom(cam, zoom, dt, zoom_speed)
	local dst_zoom = zoom - cam.zoom
	cam.zoom = cam.zoom + dst_zoom * (zoom_speed or 0.6) * dt
end

function camera.offset(cam, ox, oy)
	cam.view.x = cam.view.x + ox
	cam.view.y = cam.view.y + oy
end


return camera

local geo = {
	point = {},
	rect = {},
}

function geo.point.new(x,y)
	return {
		x = x or 0,
		y = y or 0
	}
end
function geo.rect.new(x,y,w,h)
	return {
		x = x or 0,
		y = y or 0,
		width = w or 0,
		height = h or 0
		}
end


function geo.rect.center(rect)
	return geo.point.new(rect.x + rect.width / 2, rect.y + rect.height / 2)
end


function geo.has_intersect(A, B)
	if A.width and A.height then
		-- rect and rect
		return A.x < B.x + B.width and
			A.x + A.width > B.x and
			A.y < B.y + B.height and
			A.y + A.height > B.y
	else
		-- point and rect
		return A.x > B.x and
			A.x < B.x + B.width and
			A.y > B.y and
			A.y < B.y + B.width
	end
end

function geo.rect_at(id, tilesize, numx, numy)
	local divquote = x // numx
	local divrem = x % numx
	if divrem > numy then error("logic error, exceeded height of tile source") end

	return geo.rect.new(divrem * tilesize, divquote * tilesize, tilesize, tilesize)
end

return geo


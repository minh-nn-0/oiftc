--@ Return a table contains indices to tbl that pred(entries value) return true
function table.filter(tbl, pred)
	local rs = {}
	for i,v in pairs(tbl) do
		if (pred(v)) then table.insert(rs, i) end
	end
	return rs
end

function table.set(t) -- set of list
  local u = { }
  for _, v in ipairs(t) do u[v] = true end
  return u
end

function table.find(tbl, pred) -- find index of entries v satisfying f(v)
  for i, v in pairs(tbl) do
    if pred(v) then return i end
  end
  return nil
end

function Normalize_rbga(rgba)
	local normalized = {}
	for _, v in ipairs(rgba) do
		table.insert(normalized, v / 255.0)
	end
	return normalized
end

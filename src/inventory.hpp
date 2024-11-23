#pragma once

#include <vector>
#include <array>
#include <algorithm>

namespace oiftc
{
	using inventory = std::vector<int>;

	//Hotslot will be the first slot, i.e index 0

	inline auto first_empty_slot(const inventory& iv)
	{
		return std::ranges::find(iv, -1); 
	};

	inline bool inventory_full(const inventory& iv)
	{
		return first_empty_slot(iv) != iv.end();
	};
};

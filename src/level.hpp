#pragma once

#include <Beaver/core.hpp>
#include "inventory.hpp"
namespace oiftc
{
	struct map
	{
		map(const map&) = default;
		map(const std::filesystem::path&);
		
		tiled::tilemap _tilemap;
	};

	struct level
	{
		std::vector<tiled::tilemap> _maps;
		
		beaver::sprite _player_spr;
		inventory	_player_inventory;
		std::string _name;
		unsigned _id;
		unsigned _timelimit;
	};
};

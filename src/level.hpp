#pragma once

#include <Beaver/core.hpp>

namespace mqa
{
	struct map
	{
		map(const map&) = default;
		map(const std::filesystem::path&);
		
		tiled::tilemap _tilemap;
		std::vector<int> _messes;
		
		mmath::ivec2 _player_start_pos;
		unsigned _timelimit;
	};

	struct level
	{
		unsigned _id;
		std::string _name;
		tiled::tilemap _tilemap;
	};
};

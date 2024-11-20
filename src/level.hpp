#pragma once

#include <Beaver/core.hpp>

namespace mqa
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
		
		mmath::ivec2 _player_start_pos;
		std::string _name;
		unsigned _id;
		unsigned _timelimit;
	};
};

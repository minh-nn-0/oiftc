#pragma once

#include <Beaver/core.hpp>
namespace oiftc
{
	
	// name is same as filename
	inline std::string get_level_name(const tiled::tilemap& lv)
	{
		return lv._path.filename();
	};

	inline int get_level_time_limit(const tiled::tilemap& lv)
	{
		if (lv._properties.count("time limit") == 0)
			throw std::runtime_error(
				std::format("level {} doesn't have time limit properties", get_level_name(lv)));
		return lv._properties["time limit"];
	};

	inline const tiled::objectlayer& get_data_layer(const tiled::grouplayer& layers)
	{
		return std::get<tiled::objectlayer>(*layers.get_layer_by_name("Data"));
	};

	

	struct door
	{
		std::string _from, _to;
		int _position; // X wise
	};

	inline const door* door_near_player(const std::vector<door>& doors, float player_x_pos, int range)
	{
		if (auto find_rs = std::ranges::find_if(doors, 
				[=](auto&& pos){return pos + range >= player_x_pos 
									&& pos - range <= player_x_pos;},
				&door::_position
				); find_rs != doors.end())
			return &*find_rs;
		else return nullptr;
		
	};
};

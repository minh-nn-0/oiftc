#pragma once

#include <Beaver/core.hpp>
#include "inventory.hpp"

namespace oiftc
{
	struct player
	{
		enum class STATE
		{
			IDLE,
			MOVE,
			PICKUP_WALL,
			PICKUP_GROUND,
			ACTION
		};

		beaver::sprite _spr;
		beaver::fsm<STATE> _state;
		std::vector<int> _inventory;
		int _stamina;
	};
};

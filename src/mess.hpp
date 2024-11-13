#pragma once

#include "minigame.hpp"
namespace mqa
{
	template<typename T>
	struct mess
	{
		T _target;
		int _requirement;
		minigame::type _trigger;
	};
};

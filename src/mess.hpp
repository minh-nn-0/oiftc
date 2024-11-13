#pragma once

#include "minigame.hpp"
namespace mqa
{
	template<typename T>
	struct mess
	{
		minigame::type _trigger;
		unsigned _base_score;
		int _requirement;
		T _target;
	};
};

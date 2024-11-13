#pragma once

#include <set>
#include <Beaver/time.hpp>
namespace mqa
{
	namespace minigame
	{
		struct button_prompt
		{
			std::set<int> _buttons;
			beaver::timer::countdown _timer;
		};
		struct rhythm
		{
			int _button;
			beaver::timer::countdown _interval;
		};
		struct balance
		{
			float _meter;
			float _limit;
		};
		struct rapid_tap
		{
			int _button;
			float _meter;
			float _decrease_rate;
		};

	using type = std::variant<button_prompt, rhythm, balance, rapid_tap>;
	};
};

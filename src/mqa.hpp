#ifndef M_QA_HPP
#define M_QA_HPP

#include <Beaver/core.hpp>
#include "level.hpp"
#include "mess.hpp"
#include "minigame.hpp"
#include "score.hpp"

namespace mqa
{
	struct player
	{
		beaver::sprite _spr;
	};

	struct game
	{
		game();
		void run();
		std::filesystem::path game_path();
		beaver::sdlgame _sdl;
		void run_menu();
	};

	enum class SCENES
	{
		MAIN_MENU,
		IN_GAME
	};
}


#endif

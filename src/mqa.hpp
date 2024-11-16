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

	inline std::filesystem::path game_path() {return std::filesystem::path(GAME_PATH);};
	

	enum class SCENES
	{
		MAIN_MENU,
		LEVEL_SELECTOR,
		IN_GAME,
	};
	
	struct game
	{
		game();
		void run();
		beaver::sdlgame _sdl;
		beaver::resource::manager<sdl::texture,
									sdl::font,
									sdl::music,
									sdl::soundchunk,
									tiled::tilemap> _assets;
		std::unordered_map<SCENES, beaver::gameloop> _scenes;
		int menu_selection {0};
		SCENES _current_scene;
	};

}


#endif

#include "mqa.hpp"

std::filesystem::path mqa::game::game_path()
{
	return std::filesystem::path(GAME_PATH);
};

mqa::game::game(): _sdl("MQA", 1280, 720)
{
	// Sprites
	_sdl._assets.add<sdl::texture>("sprsheet_player", 
			sdl::texture(game_path() / "assets/sprites/player.png", _sdl._sdl._renderer));
	_sdl._assets.add<sdl::texture>("sprsheet_playeraction", 
			sdl::texture(game_path() / "assets/sprites/playeraction.png", _sdl._sdl._renderer));


	// UI / Button
	
	_sdl._assets.add<sdl::texture>("keyboard",
			sdl::texture(game_path() / "assets/UI/gdb-keyboard-2.png", _sdl._sdl._renderer));
	// Audios
	

	// Fonts
};

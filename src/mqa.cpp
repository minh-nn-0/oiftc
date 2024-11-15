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
	
	_sdl._assets.add<sdl::texture>("logo",
			sdl::texture(game_path() / "assets/UI/logo.png", _sdl._sdl._renderer));
	// Audios
	
	// Fonts
};

float logo_scale {1};
bool menu_update(mqa::game& game, float dtr)
{
#ifndef NDEBUG
	ImGui::Begin("DEBUG");
	ImGui::SliderFloat("logo scale", &logo_scale, 0, 2);
	ImGui::End();
#endif

	return true;
};

void menu_draw(mqa::game& game)
{
	SDL_Renderer* rdr = game._sdl._sdl._renderer;
	SDL_SetRenderDrawColor(rdr, 0,0,0,255);
	SDL_RenderClear(rdr);
	// Draw logo
	sdl::texture* logo_tex = game._sdl._assets.get<sdl::texture>("logo");
	mmath::frect logo_dst {._size = mmath::ivec2{logo_tex->_width, logo_tex->_height}};

	logo_dst._size = logo_dst._size * logo_scale;
	logo_dst._pos = sdl::render_output_size(game._sdl._sdl) / 2 - mmath::fvec2{logo_dst._size.x/2, 40};
	sdl::draw(rdr, *game._sdl._assets.get<sdl::texture>("logo"), logo_dst);
	// Draw Buttons
};

void mqa::game::run_menu()
{
	beaver::run_game_loop(_sdl, 
			[&](float dtr){return menu_update(*this, dtr);},
			[&]{menu_draw(*this);});
};

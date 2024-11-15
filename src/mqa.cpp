#include "mqa.hpp"

void load_assets(mqa::game& game)
{
	SDL_Renderer* rdr = game._sdl._sdl._renderer;
	std::ifstream f{mqa::game_path() / "data/assets"};
	std::string lines;

	
	std::string type;
	while (std::getline(f, lines))
	{
		if (lines.starts_with('#'))
		{
			if (lines.starts_with("#Texture")) type = "texture";
			else if (lines.starts_with("#Audios")) type = "audio";
			else if (lines.starts_with("#Music")) type = "music";
			else if (lines.starts_with("#Fonts")) type = "font";
			else if (lines.starts_with("#Maps")) type = "map";
		}
		else
		{
			if (!lines.empty())
			{
				std::stringstream fields {lines};
				std::string name, path;
				fields >> name; fields >> path;
				if (!std::filesystem::exists(mqa::game_path() / path))
					throw std::runtime_error(std::format("{} not found", path));

				path = mqa::game_path() / path;
				if (type == "texture") 
					game._assets.add<sdl::texture>(name, sdl::texture{path, rdr});
				else if (type == "audio")
					game._assets.add<sdl::soundchunk>(name, sdl::soundchunk{path.c_str()});
				else if (type == "music")
					game._assets.add<sdl::music>(name, sdl::music{path.c_str()});
				else if (type == "font")
					game._assets.add<sdl::font>(name, sdl::font{path.c_str(), 80});
				else if (type == "map")
					game._assets.add<tiled::tilemap>(name, tiled::tilemap{path});

				std::println("adding assets \"{}\" \n\tpath \"{}\",\n\ttype \"{}\"", name, path, type);
			};
		};
	};
};
mqa::game::game(): _sdl("MQA", 1280, 720)
{
	load_assets(*this);
	//// Sprites
	//_sdl._assets.add<sdl::texture>("sprsheet_player", 
	//		sdl::texture(game_path() / "assets/sprites/player.png", _sdl._sdl._renderer));
	//_sdl._assets.add<sdl::texture>("sprsheet_playeraction", 
	//		sdl::texture(game_path() / "assets/sprites/playeraction.png", _sdl._sdl._renderer));


	//// UI / Button
	//
	//_sdl._assets.add<sdl::texture>("keyboard",
	//		sdl::texture(game_path() / "assets/UI/gdb-keyboard-2.png", _sdl._sdl._renderer));
	//
	//_sdl._assets.add<sdl::texture>("logo",
	//		sdl::texture(game_path() / "assets/UI/logo_small.png", _sdl._sdl._renderer));

	//// Audios
	//
	//// Fonts
};

float logo_scale {1};
int menu_selection {0};
bool menu_update(mqa::game& game, float dtr)
{
#ifndef NDEBUG
	ImGui::Begin("DEBUG");
	ImGui::SliderFloat("logo scale", &logo_scale, 0, 2);
	ImGui::End();
#endif


	if (game._sdl._ctl.just_pressed(beaver::BTND)) menu_selection++;
	if (game._sdl._ctl.just_pressed(beaver::BTNU)) menu_selection--;
	menu_selection = std::clamp(0, menu_selection, 3);

	return true;
};

const std::vector<std::string> MENU_TEXT {"Start", "Options", "Credits", "Quit"};
void menu_draw(mqa::game& game)
{
	SDL_Renderer* rdr = game._sdl._sdl._renderer;
	SDL_SetRenderDrawColor(rdr, 0,0,0,255);
	SDL_RenderClear(rdr);

	// Draw background
	sdl::draw(rdr, *game._assets.get<sdl::texture>("menu_bg2"), {});
	// Draw logo
	sdl::texture* logo_tex = game._assets.get<sdl::texture>("logo");
	mmath::frect logo_dst {._size = mmath::ivec2{logo_tex->_width, logo_tex->_height}};

	logo_dst._size = logo_dst._size * logo_scale;
	logo_dst._pos = {sdl::render_output_size(game._sdl._sdl).x/2.f - logo_dst._size.x/2.f, 20};

	sdl::draw(rdr, *game._assets.get<sdl::texture>("logo"), logo_dst);
	// Draw menu text
	
	float start_ypos {logo_dst._pos.y + logo_dst._size.y + 40};
	for (int i {0}; i != 4; i++)
	{
		sdl::font* modak = game._assets.get<sdl::font>("cnr");
		SDL_Surface* temp_surf = TTF_RenderUTF8_Solid(*modak, MENU_TEXT[i].c_str(), {0,0,0,255});

		sdl::texture text_tex {SDL_CreateTextureFromSurface(rdr,temp_surf)};
		SDL_FreeSurface(temp_surf);

		float text_ypos = start_ypos + 40 * i;
		mmath::fvec2 dst_size = mmath::ivec2{text_tex._width, text_tex._height} / 2;
		if (menu_selection == i) dst_size = dst_size * 1.5;
		mmath::frect dst = {._pos = {sdl::render_output_size(game._sdl._sdl).x/2.f - dst_size.x/2.f, text_ypos - dst_size.y/2.f},
							._size = dst_size};

		sdl::draw(rdr, text_tex, dst);
	};
};

void mqa::game::run_menu()
{
	beaver::run_game_loop(_sdl, 
			[&](float dtr){return menu_update(*this, dtr);},
			[&]{menu_draw(*this);});
};

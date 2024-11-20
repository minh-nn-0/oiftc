#include "oiftc.hpp"

//std::vector<sdl::texture*> MENU_BUTTON;

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
					game._assets.add<sdl::font>(name, sdl::font{path.c_str(), 120});
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

	//_states._cur = SCENES::MAIN_MENU;

	//_states._transition_table[SCENES::MAIN_MENU][SCENES::LEVEL_SELECTOR] =
	//	[&]
	//	{
	//		
	//	};
	//_states._transition_table[SCENES::LEVEL_SELECTOR][SCENES::MAIN_MENU] =
	//	[&]
	//	{
	//		
	//	};
	//_states._transition_table[SCENES::LEVEL_SELECTOR][SCENES::IN_GAME] =
	//	[&]
	//	{
	//		
	//	};


	_scenes[SCENES::MAIN_MENU] = 
	{
		._initf = []{std::println("init mainmenu");},
		._updatef = [&](float dtr)
		{
			if (_sdl._ctl.just_pressed(beaver::BTND)) menu_selection++;
			if (_sdl._ctl.just_pressed(beaver::BTNU)) menu_selection--;
			menu_selection = std::clamp(0, menu_selection, 3);

			if (_sdl._ctl.just_pressed(beaver::BTNZ))
			{
				if (menu_selection == 0)
				{
					_current_scene = SCENES::LEVEL_SELECTOR;
					return false;
				};
				if (menu_selection == 3) 
				{
					_sdl._running = false;
					return false;
				};
			};
			return true;
		},
		._drawf = [&]()
		{
			SDL_Renderer* rdr = _sdl._sdl._renderer;
			static const std::vector<std::string> MENU_TEXT {"Start", "Options", "Credits", "Quit"};
			SDL_SetRenderDrawColor(rdr, 255,0,0,255);
			SDL_RenderClear(rdr);

			// Draw background
			sdl::draw(rdr, *_assets.get<sdl::texture>("menu_bg2"), {});

			// Draw logo
			sdl::texture* logo_tex = _assets.get<sdl::texture>("logo");
			mmath::frect logo_dst {._size = mmath::ivec2{logo_tex->_width, logo_tex->_height}};

			logo_dst._pos = {sdl::render_output_size(_sdl._sdl).x/2.f - logo_dst._size.x/2.f, 20};

			sdl::draw(rdr, *_assets.get<sdl::texture>("logo"), logo_dst);
			// Draw menu text
			
			float start_ypos {logo_dst._pos.y + logo_dst._size.y + 40};
			for (int i {0}; i != 4; i++)
			{
				sdl::font* modak = _assets.get<sdl::font>("cnr");
				sdl::texture text_tex {sdl::render_text_solid(rdr, *modak, MENU_TEXT[i], {0,0,0,255})};

				mmath::fvec2 dst_size = mmath::ivec2{text_tex._width, text_tex._height} / 3;
				if (menu_selection == i) dst_size = dst_size * 1.5;
				
				float text_ypos = start_ypos + 40 * i;
				mmath::frect dst = {._pos = {sdl::render_output_size(_sdl._sdl).x/2.f - dst_size.x/2.f,
											text_ypos - dst_size.y/2.f},
									._size = dst_size};

				sdl::draw(rdr, text_tex, dst);
			};

		},
		._exitf = []{std::println("exit mainmenu");}
	};




	_current_scene = SCENES::MAIN_MENU;
};

void mqa::game::run()
{
	while (_sdl._running)
	{
		beaver::run_game_loop(_sdl, _scenes[_current_scene]);
	};
};

#include "mqa.hpp"
#include <random>

using namespace mqa;
std::default_random_engine rde;

std::uniform_real_distribution target_dist{23.f, 440.f};
std::bernoulli_distribution cat_walk_from_idle {0.4};
std::bernoulli_distribution cat_run_from_idle {0.3};
std::bernoulli_distribution cat_lick_from_idle {0.1};
std::bernoulli_distribution cat_idle_from_lick {0.2};
std::bernoulli_distribution cat_want_to_switch_room {0.4};


beaver::sdlgame maingame {"mqa", 1280, 720};

SDL_Renderer* rdr = maingame._sdl._renderer;

// ASSETS

sdl::texture main_tileset {(std::string(ASSETS_PATH) + "tilemap_all.png").c_str(), rdr};
const sdl::font pps_font {TTF_OpenFont((std::string(ASSETS_PATH) + "playpensans.ttf").c_str(), 24)};
const sdl::texture playertilesheet {(std::string(ASSETS_PATH) + "player.png").c_str(), rdr};
const sdl::texture cat_tilesheet {(std::string(ASSETS_PATH) + "cats.png").c_str(),rdr};

const sdl::texture leftswitchnote_text(SDL_CreateTextureFromSurface(rdr, 
			TTF_RenderUTF8_Shaded(pps_font, "<- Ghi chú trước", {255, 255, 255, 255}, {40, 40, 40, 255})));
const sdl::texture rightswitchnote_text(SDL_CreateTextureFromSurface(rdr, 
		TTF_RenderUTF8_Shaded(pps_font, "Ghi chú sau ->", {255, 255, 255, 255}, {40, 40, 40, 255})));


sdl::texture make_white_text(const std::string& text)
{
	return sdl::texture{SDL_CreateTextureFromSurface(rdr, TTF_RenderUTF8_Solid(pps_font, text.c_str(), {0,0,0,225}))};
};


const beaver::tile_animation::framevec p_anm_idle
{ {150, 22},{150,23},{150, 24},{150,25},{150, 26},{150, 27},{150, 28},{150, 29} };

const beaver::tile_animation::framevec p_anm_walk
{ {150, 92},{150, 93},{150, 94},{150, 95},{150,95},{150, 97},{150, 98},{150, 99} };

const beaver::tile_animation::framevec p_anm_jump
{ {150, 30},{150, 31},{150, 20},{150, 21},{150,32},{150, 33} };

const beaver::tile_animation::framevec p_anm_attack
{ {150, 53},{150,54},{150, 55},{150,56},{150,57} };

const beaver::tile_animation::framevec  p_anm_pickup_ground
{ {150, 35},{150,36},{150, 37},{150,38}, {150, 39} };

const beaver::tile_animation::framevec p_anm_pickup_side
{ {150, 40}, {150, 41}, {150, 42}, {150, 43}, {150, 44}, {150, 45},
	{150, 46}, {150, 47}, {150, 48}, {150, 49}, {150, 50}, {150, 51} };

const beaver::tile_animation::framevec p_anm_lay
{ {400, 11}, {400,18} };

const beaver::tile_animation::framevec cat_anm_idle {{150, 0}, {150, 1}, {150, 2}, {150, 3}, {150, 4}, {150, 5}, {150, 6},{150, 7},{150, 8},{150, 9}};
const beaver::tile_animation::framevec cat_anm_walk {{150, 24}, {150, 25}, {150, 26}, {150, 27}, {150, 28}, {150, 29},{150, 30},{150, 31}};
const beaver::tile_animation::framevec cat_anm_run {{150, 16}, {150, 17}, {150, 18}, {150, 19}, {150, 20}, {150, 21}, {150, 22},{150, 23}};
const beaver::tile_animation::framevec cat_anm_lick {{150, 10}, {150, 11}, {150, 12}, {150, 13}, {150, 14}};
const beaver::tile_animation::framevec cat_anm_lay {{150, 15}};

// ITEM COLLECTING
const std::vector<int> bed_items {413, 414};
const std::vector<int> bath_items {604, 1090, 1327, 1289};

// Tilemap
tiled::tilemap house_tm {(std::string(ASSETS_PATH) + "pqa2.tmj").c_str()};
const std::vector<tiled::layer> map_data {house_tm._layersdata};
auto& item_layer = std::get<tiled::tilelayer>(house_tm.get_layer_by_name("ITEMS"))._data;

std::map<mmath::fvec2, std::map<int, int>> itemmap1_m;
std::map<mmath::fvec2, std::map<int,int>> itemmap1_l;
std::map<mmath::fvec2, std::map<int, int>> itemmap2_m;
std::map<mmath::fvec2, std::map<int, int>> itemmap2_l;

// CAMERA
beaver::camera2D cam {._view = {0, 0, 400, 400}, ._zoom = 4};



// PLAYER STATE
enum class player_state
{
	idle,
	moveleft,
	moveright,
	jump,
	interact_medium,
	interact_low,
	attack,
	hitbycat,
};
//CAT STATE
enum class catstate
{
	idle,
	walk,
	run,
	lick,
};

// PLAYERDATA
state<player_state> p_state {player_state::idle, player_state::idle};
beaver::tile_animation p_anm {p_anm_idle};
mmath::frect p_rect {200, 64, 64, 64};
bool p_canmove, p_in_bathroom, p_step{false};
unsigned flipflags {};


// CATDATA
beaver::tile_animation cat_anm {cat_anm_idle};
state<catstate> cat_state {catstate::idle, catstate::idle};
mmath::frect cat_rect {100, 80, 50, 50};
mmath::fvec2 cat_move_target{ 230, 80};
bool cat_moving {true}, cat_switching_room {false}, cat_facing_right{true}, cat_in_bathroom {false};
unsigned cat_flipflags {};



//NOTES

std::map<int, sdl::texture> notes_texture;
std::vector<int> notes_in_hand{0};
int note_index {0};
bool note_opening;

void open_note()
{
	note_opening = !note_opening;
};

void draw_note(const sdl::texture& note)
{
	mmath::ivec2 screen;
	SDL_GetRendererOutputSize(rdr, &screen.x, &screen.y);
	
	mmath::ivec2 screencenter = screen / 2;
	

	sdl::draw(rdr, note,  mmath::frect{screencenter.x - note._width/2.f, 
								screencenter.y - note._height/2.f, 
								static_cast<float>(note._width),
								static_cast<float>(note._height)});
	if (!(note_index == 0)) 
		sdl::draw(rdr, leftswitchnote_text, 
					mmath::frect{ 20, screen.y - 60.f,
								static_cast<float>(leftswitchnote_text._width),
								static_cast<float>(leftswitchnote_text._height)});
	if (!(note_index == notes_in_hand.size()-1)) 
		sdl::draw(rdr, rightswitchnote_text, 
					mmath::frect{ screen.x - rightswitchnote_text._width - 20.f,
								screen.y - 60.f,
								static_cast<float>(rightswitchnote_text._width),
								static_cast<float>(rightswitchnote_text._height)});
													
};


// INVENTORY
std::vector<int> inventory;
int itemcount_bed, itemcount_bath, chorecount;

void draw_inventory()
{
	sdl::texture text_inventory(SDL_CreateTextureFromSurface(rdr, 
				TTF_RenderUTF8_Solid(pps_font, "Đồ cần dọn đang mang trên người", {0,0,0,255})));

	SDL_Rect dstinventory = {10, 10, text_inventory._width, text_inventory._height};
	SDL_RenderCopy(rdr, text_inventory, NULL, &dstinventory);
	
	int iposx {10}, iposy {text_inventory._height + 10};

	for (auto item: inventory)
	{
		const auto& ts = std::ranges::find_if(house_tm._tilesets | std::views::reverse,
				[&](auto&& ts){return ts.first <= item;})->second;
		mmath::frect dst = {static_cast<float>(iposx), static_cast<float>(iposy), 64, 64};
		
		sdl::draw(rdr, main_tileset, tiled::rect_at(item, ts), dst);
		iposx += 74;
	};
};

// Countdown timer
timer maintimer {120};
timer p_lay_timer {2};

void draw_time(const timer& t)
{
	auto time_remaining = std::div(t._limit - t._elapsed, 60);
	std::string time_text = std::format("{}:{}", time_remaining.quot, time_remaining.rem);
	sdl::texture time_texture {SDL_CreateTextureFromSurface(rdr,
							TTF_RenderUTF8_Shaded(pps_font, time_text.c_str(), {255, 255, 255, 255}, {20, 20, 20, 255}))};

	mmath::ivec2 screen;
	SDL_GetRendererOutputSize(rdr, &screen.x, &screen.y);
	
	sdl::draw(rdr, time_texture, 
			mmath::irect{screen.x - time_texture._width - 30,
						time_texture._height + 20,
						time_texture._width,
						time_texture._height});
};

//TIMING
std::chrono::steady_clock cl;
constexpr int FPS = 60;

auto prev_frame = cl.now(), cur_frame = cl.now();
const auto elapsed_time = [](){return std::chrono::duration_cast<std::chrono::milliseconds>(cur_frame - prev_frame).count();};
const auto new_frame_should_start = [](){cur_frame = cl.now(); return elapsed_time() >= 1000.0/FPS;};
const auto end_frame = [](){prev_frame = cur_frame;};

const auto process_sdl_event = [](bool& process)
{
	SDL_Event e;
	maingame._ctl.swap();
	while(SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT) process = false;
		maingame._ctl.update(e);
	};
};


Mix_Music* mainbgm = Mix_LoadMUS((std::string(ASSETS_PATH) + "bgm1.mp3").c_str());
Mix_Chunk* step = Mix_LoadWAV((std::string(ASSETS_PATH) + "step.mp3").c_str());
Mix_Chunk* notif = Mix_LoadWAV((std::string(ASSETS_PATH) + "notif.mp3").c_str());
Mix_Chunk* door = Mix_LoadWAV((std::string(ASSETS_PATH) + "door.mp3").c_str());
Mix_Chunk* hit = Mix_LoadWAV((std::string(ASSETS_PATH) + "hit.mp3").c_str());

void p_update(const float& dt)
{
	auto move_left = [&]()
	{	
		p_rect._pos.x -= 1 * dt;
		flipflags = SDL_FLIP_HORIZONTAL;
		if (state_just_change(p_state, player_state::moveleft)) p_anm = {._frames = p_anm_walk};
	};
	auto move_right = [&]()
	{
		p_rect._pos.x += 1 * dt;
		flipflags = SDL_FLIP_NONE;
		if (state_just_change(p_state, player_state::moveright)) p_anm = {._frames = p_anm_walk};
	};
	auto pickup_ground = [&]()
	{
		p_anm = {._frames = p_anm_pickup_ground, ._repeat = false};
		p_canmove = false;
	};
	auto interact = [&]()
	{
		p_anm = {._frames = p_anm_pickup_side, ._repeat = false};
		p_canmove = false;
	};
	auto attack = [&]()
	{
		p_anm = {._frames = p_anm_attack, ._repeat = false};
		p_canmove = false;
	};
	auto jump = [&]()
	{
		p_anm = {._frames = p_anm_jump, ._repeat = false};
		p_canmove = false;
	};

	auto hit_by_cat = [&]()
	{
		return (state_is(cat_state, catstate::run)
			&& !state_is(p_state, player_state::jump)
			&& std::abs(cat_rect.center().x - p_rect.center().x) < 2
			&& (p_in_bathroom == cat_in_bathroom));
	};
	

	// Limit player pos
	if (!p_in_bathroom)
		p_rect._pos.x = std::clamp(p_rect._pos.x, -22.f, 438.f);
	else p_rect._pos.x = std::clamp(p_rect._pos.x, 156.f, 438.f);

	// State handling
	state_reserve(p_state);
	
	if (note_opening)
	{
		p_canmove = false;
		state_set(p_state, player_state::idle);
	}

	if (!p_canmove)
	{
		if (!note_opening && !state_is(p_state, player_state::hitbycat)
				&& (state_is(p_state, player_state::idle) || !p_anm._playing))
			p_canmove = true;
	}

	if (hit_by_cat() && !state_is(p_state, player_state::hitbycat))
	{
		state_set(p_state, player_state::hitbycat);
		p_lay_timer.reset();
		p_canmove = false;
		Mix_PlayChannel(-1, hit, 0);
	};
	
	if (state_is(p_state, player_state::hitbycat))
	{
		p_lay_timer.update(dt);
		if (!p_lay_timer.running()) p_canmove = true;
	};

	if (!(state_is(p_state, player_state::moveleft) && (state_is(p_state, player_state::moveright)))
		&& p_canmove)
		state_set(p_state, player_state::idle);
	
	if (maingame._ctl.pressed(beaver::BTNL) && p_canmove) 
		state_set(p_state, player_state::moveleft);
	
	if (maingame._ctl.pressed(beaver::BTNR) && p_canmove) 
		state_set(p_state, player_state::moveright);
	
	if (state_is(p_state, player_state::idle))
	{
		if (maingame._ctl.just_pressed(beaver::BTNU)) state_set(p_state,player_state::jump);
		if (maingame._ctl.just_pressed(beaver::BTND)) state_set(p_state,player_state::interact_low);
		if (maingame._ctl.just_pressed(beaver::BTNX)) state_set(p_state,player_state::interact_medium);
		if (maingame._ctl.just_pressed(beaver::BTNC)) state_set(p_state,player_state::attack);
		if (maingame._ctl.just_pressed(beaver::BTNZ)) 
		{
			auto& p_pos = p_rect._pos;
			if (!p_in_bathroom) 
			{
				if (p_rect.center().x < 262 && p_rect.center().x > 252)
				{
					Mix_PlayChannel(-1, door, 0);
					p_pos = {159, 240};
					p_in_bathroom = !p_in_bathroom;
				};
			}
			else if (p_rect.center().x < 196 && p_rect.center().x > 186)
			{
				Mix_PlayChannel(-1, door, 0);
				p_pos = {222, 64};
				p_in_bathroom = !p_in_bathroom;
			};
			
				
		};
	};


	if (state_is(p_state, player_state::moveleft) && p_canmove) move_left();
	else if (state_is(p_state, player_state::moveright) && p_canmove) move_right();
	else if (state_just_change(p_state, player_state::interact_low)) pickup_ground();
	else if (state_just_change(p_state, player_state::interact_medium)) interact();
	else if (state_just_change(p_state, player_state::attack)) attack();
	else if (state_just_change(p_state, player_state::jump)) jump();
	else if (state_just_change(p_state, player_state::idle)) p_anm = {p_anm_idle};
	else if (state_just_change(p_state, player_state::hitbycat)) p_anm = {._frames = p_anm_lay, ._repeat = false};
	


	p_anm.update(dt);
	auto p_is_stepping = [&](){return p_anm._current_frame->_id == 93 || p_anm._current_frame->_id == 97;};
	if (state_is(p_state, player_state::moveleft) || state_is(p_state, player_state::moveright))
	{
		//We don't want to play sound everytime frame changed ;(
		//We want on specific frame
		//
		if (p_is_stepping()) 
		{
			if (!p_step)
			{
				Mix_PlayChannelTimed(-1, step, 0, 500);
				p_step = true;
			};
		}
		else p_step = false;
	};
		
};
void put_to_bed()
{
	bool any_item_put_in {false};
	for (auto& needed: bed_items)
		if (auto item = std::ranges::find(inventory, needed); item != inventory.end() && !p_anm._playing)
		{
			any_item_put_in = true;
			itemcount_bed++;
			inventory.erase(item);
		}; 

	if (any_item_put_in) Mix_PlayChannel(-1, notif, 0);
};

void put_to_bath()
{
	bool any_item_put_in {false};
	for (auto& needed: bath_items)
		if (auto item = std::ranges::find(inventory, needed); item != inventory.end() && !p_anm._playing)
		{
			any_item_put_in = true;
			itemcount_bath++;
			inventory.erase(item);
		}; 
	if (any_item_put_in) Mix_PlayChannel(-1, notif, 0);
};

void try_picking()
{
	auto& itemmap = p_in_bathroom ? itemmap2_l : itemmap1_l;
	const mmath::fvec2& p_pos {p_rect.center()};

	if (auto item_pickedup = std::ranges::find_if(itemmap, [&](auto&& it)
				{
					return (it.first.x < p_pos.x && it.first.y > p_pos.x);
				}); item_pickedup != itemmap.end() && !p_anm._playing)
	{
		Mix_PlayChannel(-1, notif, 0);
		for (auto [pos, newtileid]: item_pickedup->second)
		{
			if (std::ranges::find(bed_items, item_layer.at(pos)) != bed_items.end()
				|| std::ranges::find(bath_items, item_layer.at(pos)) != bath_items.end())
				inventory.push_back(item_layer.at(pos));
			
			item_layer.at(pos) = newtileid;
		};
		itemmap.erase(item_pickedup);
	};


	if (auto note_pickedup = std::ranges::find_if(notes_texture,[](auto&& it)
				{
					mmath::irect note_rect = tiled::rect_at(it.first, house_tm);
					return (note_rect._pos.x < p_rect.center().x && note_rect._pos.x + note_rect.width() > p_rect.center().x
							&& std::abs(note_rect._pos.y - p_rect.center().y) < 10);
				}); note_pickedup != notes_texture.end() && !p_anm._playing)
	{
		if (std::ranges::find(notes_in_hand, note_pickedup->first) == notes_in_hand.end())
		{
			Mix_PlayChannel(-1, notif, 0);
			notes_in_hand.push_back(note_pickedup->first);
			note_index = notes_in_hand.size()-1;
			open_note();
		};
	};

};

void try_interacting()
{
	auto& itemmap = p_in_bathroom ? itemmap2_m : itemmap1_m;
	const mmath::fvec2& p_pos {p_rect.center()};

	if (auto item_pickedup = std::ranges::find_if(itemmap, [&](auto&& it)
				{
					return (it.first.x < p_rect.center().x && it.first.y > p_rect.center().x);
				}); item_pickedup != itemmap.end() && !p_anm._playing)
	{
		if (!p_in_bathroom) chorecount++;
		Mix_PlayChannel(-1, notif, 0);
		for (auto [pos, newtileid]: item_pickedup->second)
		{
			if (std::ranges::find(bed_items, item_layer.at(pos)) != bed_items.end()
				|| std::ranges::find(bath_items, item_layer.at(pos)) != bath_items.end())
				inventory.push_back(item_layer.at(pos));
			
			item_layer.at(pos) = newtileid;
		};
		itemmap.erase(item_pickedup);
	};

	if (!p_in_bathroom) 
	{
		if (p_pos.x < 470 && p_pos.x > 403) put_to_bed();
	}
	else if (p_pos.x < 255 && p_pos.x > 242) put_to_bath();
};

void p_draw()
{
	sdl::draw(rdr, playertilesheet, tiled::rect_at(p_anm._current_frame->_id, 64, 10, 10), 
								mmath::frect{p_rect._pos.x - cam._view._pos.x,
											p_rect._pos.y - cam._view._pos.y,
											p_rect.width(), p_rect.height()},
								flipflags);
};

void cat_update(const float& dt)
{
	auto new_target = [&]()
	{
		cat_in_bathroom ? cat_move_target = {std::max(210.f, target_dist(rde)), 256} 
						: cat_move_target = {target_dist(rde), 80};
	};
	
	auto hit_target = [](){return std::abs(cat_move_target.x - cat_rect.center().x) < 1.5;};
	state_reserve(cat_state);

	if (hit_target())
	{
		state_set(cat_state, catstate::idle);
		if (cat_switching_room)
		{
			auto& cat_pos = cat_rect._pos;
			if (cat_in_bathroom)
			{
				cat_pos = {203, 80};
			}
			else cat_pos = {139, 256};

			cat_in_bathroom = !cat_in_bathroom;

			cat_switching_room = false;
			
			Mix_PlayChannel(-1, door, 0);
		}
		else if (cat_want_to_switch_room(rde))
		{
			cat_in_bathroom ? 
				cat_move_target.x = 195
				: cat_move_target.x = 255;
			cat_switching_room = true;
		}
		else new_target();
	};

	if (state_is(cat_state,catstate::idle))
	{
		if (cat_walk_from_idle(rde)) state_set(cat_state, catstate::walk);
		else if (cat_run_from_idle(rde)) state_set(cat_state, catstate::run);
		else if (cat_lick_from_idle(rde)) state_set(cat_state, catstate::lick);
	};


	if (state_is(cat_state, catstate::lick))
	{
		if (cat_anm.frame_is_end())
		{
			if (cat_idle_from_lick(rde)) state_set(cat_state, catstate::idle);
		};
	};

	//if (state_is(cat_state,catstate::walk))
	//{
	//	if (cat_run_dist(rde)) state_set(cat_state,catstate::run);
	//};

	//if (state_is(cat_state, catstate::run))
	//{
	//	if (std::abs(cat_move_target.x - cat_rect.center().x) < 1.5)
	//	{
	//		state_set(cat_state, catstate::idle);
	//		new_target();
	//	};
	//};

	if (state_just_change(cat_state, catstate::idle)) cat_anm = {cat_anm_idle};
	if (state_just_change(cat_state, catstate::walk)) cat_anm = {cat_anm_walk};
	if (state_just_change(cat_state, catstate::run)) cat_anm = {cat_anm_run};
	if (state_just_change(cat_state, catstate::lick)) cat_anm = {cat_anm_lick};


	cat_move_target.x - cat_rect.center().x > 0 ?
		cat_facing_right = true
		: cat_facing_right = false;

	if (state_is(cat_state, catstate::walk))
	{
		cat_facing_right ? 
			cat_rect._pos.x += 1 * dt 
			: cat_rect._pos.x -= 1 * dt;
	};

	if (state_is(cat_state, catstate::run))
	{
		cat_facing_right ? 
			cat_rect._pos.x += 2 * dt 
			: cat_rect._pos.x -= 2 * dt;
	};
	
	// Limit cat pos
	if (!cat_in_bathroom)
		cat_rect._pos.x = std::clamp(cat_rect._pos.x, -12.f, 443.f);
	else cat_rect._pos.x = std::clamp(cat_rect._pos.x, 161.f, 443.f);
	
	cat_facing_right ? 
		cat_flipflags = SDL_FLIP_NONE
		: cat_flipflags |= SDL_FLIP_HORIZONTAL;
	cat_anm.update(dt);

};

void cat_draw()
{
	sdl::draw(rdr, cat_tilesheet, tiled::rect_at(cat_anm._current_frame->_id, 50, 16, 2), 
							mmath::frect{cat_rect._pos.x - cam._view._pos.x,
										cat_rect._pos.y - cam._view._pos.y,
										cat_rect._size.x, cat_rect._size.y},
							cat_flipflags);
};

void gamereset()
{
	house_tm._layersdata = map_data;
	p_rect = {200,64,64,64};
	p_canmove = true;
	p_in_bathroom = false;
	itemcount_bed = itemcount_bath = chorecount = 0;
	inventory.clear();
	notes_in_hand.clear(); notes_in_hand.push_back(0);
	maintimer.reset();
	
	itemmap1_m = {{{26, 40}, {{151, 667}, {152, 668}}},
				{{200, 230}, {{192, 1280},{193,1281}, {194, 1282}}},

				{{155, 165}, {{128, 0},{129,440},{130,441},
								 {158, 0}, {159, 478}, {160, 479},
								{188, 0}, {189, 516}, {190, 517}}},

				{{436, 466}, {{146, 0},{147,1315},{148,1316}, {149, 1317},
								 {176, 0}, {177, 1353}, {178, 1354}, {179, 1355},
								{206, 0}, {207, 1391}, {208, 1392}, {209, 1393}}}};

	itemmap1_l = { {{10, 15}, {{180,639}}}, 
					{{275, 285}, {{197, 0}}},
					{{288, 300}, {{198, 0}}}};
	itemmap2_m = { {{260, 270}, {{496, 0}}} };
	itemmap2_l = { {{210, 223}, {{523, 0}}},
					{{370, 380}, {{533, 0}}},
					{{387, 397}, {{534, 0}}},
					{{400, 410}, {{535, 0}}} };
};

void greeting()
{
	bool greeting {true};
	while (greeting)
	{
		process_sdl_event(greeting);
		
		if (maingame._ctl.just_pressed(beaver::BTNV)) greeting = false;

		auto [r,g,b,a] = house_tm._bgcolor;
		SDL_SetRenderDrawColor(rdr, r, g, b, a);
		SDL_RenderClear(rdr);

		draw_note(notes_texture[0]);
		
		SDL_RenderPresent(rdr);
	};
};

void main_loop();
void game_over()
{
	std::cout << "GAMEOVER\n";
	
	std::vector<sdl::texture> gameovertext;
	gameovertext.emplace_back(make_white_text(std::format("Số việc nhà đã làm: {}/4", chorecount)));
	gameovertext.emplace_back(make_white_text(std::format("Số đồ trên giường đã dọn: {}/3", itemcount_bed)));
	gameovertext.emplace_back(make_white_text(std::format("Số đồ nhà tắm đã dọn: {}/4", itemcount_bath)));
	gameovertext.emplace_back(make_white_text(std::format("Số note tìm thấy: {}/4", notes_in_hand.size() - 1)));
	gameovertext.emplace_back(make_white_text("Nhấn Q để thử lại"));
	gameovertext.emplace_back(make_white_text("Nhấn Escape để thoát"));

	bool gameover {true};
	bool tryagain {false};
	while(gameover)
	{
		process_sdl_event(gameover);
		if (maingame._ctl.just_pressed(beaver::BTNE)) gameover = false; 
		if (maingame._ctl.just_pressed(beaver::BTNO)) 
		{
			gameover = false;
			tryagain = true;
		};

		auto [r,g,b,a] = house_tm._bgcolor;
		SDL_SetRenderDrawColor(rdr, r, g, b, a);
		SDL_RenderClear(rdr);
		
		for (int i = 0; i != gameovertext.size(); i++)
		{
			const sdl::texture& text = gameovertext[i];
			mmath::ivec2 screen;

			SDL_GetRendererOutputSize(rdr, &screen.x, &screen.y);
			
			sdl::draw(rdr, text, mmath::irect {screen.x/2 - text._width/2,
									 i*30 + 20,
									 text._width,
									 text._height});
		};
		SDL_RenderPresent(rdr);
	};

	if (tryagain) main_loop();
};

void main_loop()
{
	gamereset();
	prev_frame = cur_frame = cl.now();
	bool running {true};

	std::vector<sdl::texture*> ts;

	ts.push_back(&main_tileset);
	while (running)
	{
		cur_frame = cl.now();
		if (float dt = elapsed_time(); dt >= 1000.0/FPS)
		{
			dt /= 17;
			maintimer.update(dt);;
			process_sdl_event(running);
			
			if (maingame._ctl.just_pressed(beaver::BTNV)) open_note();
			if (maingame._ctl.just_pressed(beaver::BTNO)) running = false;
			
			//UPDATE
			if (note_opening)
			{
				if (maingame._ctl.just_pressed(beaver::BTNR)) note_index++; 
				if (maingame._ctl.just_pressed(beaver::BTNL)) note_index--; 
				
				note_index = std::clamp(note_index, 0, static_cast<int>(notes_in_hand.size()) - 1);
			};

			// camera
			int rw, rh;
			SDL_GetRendererOutputSize(rdr, &rw, &rh);
			cam._view._size.x = rw;
			cam._view._size.y = rh;
			
			cam.target(p_rect.center(), dt);

			if (!maintimer.running()) running = false;
			
			// update player state and cat state
			p_update(dt);
			cat_update(dt);
			
			// player interacting 
			if (state_is(p_state, player_state::interact_low)) try_picking();
			if (state_is(p_state, player_state::interact_medium)) try_interacting();

			//DRAW
			auto [r,g,b,a] = house_tm._bgcolor;
			SDL_SetRenderDrawColor(rdr, r, g, b, a);
			SDL_RenderClear(rdr);
			SDL_RenderSetScale(rdr, cam._zoom, cam._zoom);
			
			// draw game world
			
			sdl::draw_tilemap(rdr, cam, house_tm, ts);
			cat_draw();
			p_draw();

			// draw UI, Note,...
			SDL_RenderSetScale(rdr, 1.0, 1.0);
			if (note_opening) draw_note(notes_texture.at(notes_in_hand.at(note_index)));
			draw_inventory();
			draw_time(maintimer);

			//// Imgui
			//ImGui_ImplSDLRenderer2_NewFrame();
			//ImGui_ImplSDL2_NewFrame();
			//ImGui::NewFrame();

			//ImGui::Begin("CAMERA");
			//ImGui::SliderFloat("cam x", &cam._view._pos.x, -1000, 1000);
			//ImGui::SliderFloat("cam y", &cam._view._pos.y, -1000, 1000);
			//ImGui::Text("cam cx %f, cam cy %f", cam._view.center().x, cam._view.center().y);
			//ImGui::Text("cam w %f, cam h %f", cam._view.width(), cam._view.height());
			//ImGui::Text("pcx %f,  pcy %f", p_rect.center().x, p_rect.center().y);
			//ImGui::Text("animation current frame %d", p_anm._current_frame->_id);
			//ImGui::Text("itembed %d, itembath %d", itemcount_bed, itemcount_bath);
			//ImGui::Text("main timer %f", maintimer._elapsed);
			//ImGui::Text("p lay timer %f", p_lay_timer._elapsed);
			//ImGui::Text("target x %f, target y %f", cat_move_target.x, cat_move_target.y);
			//if (ImGui::Button("note")) note_opening = !note_opening;
			//ImGui::End();


			//ImGui::Begin("NOTES");
			//for (auto& note_id: notes_texture)
			//{
			//	mmath::ivec2 notepos = tiled::rect_at(note_id.first, house_tm).side_h();
			//	ImGui::Text("noteid: %d, noteposx1 %d noteposx2 %d", note_id.first, notepos.x, notepos.y);
			//};
			//ImGui::End();
			//ImGui::Render();
			//ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), rdr);
			//

			SDL_RenderPresent(rdr);
			end_frame();
		};
	};
	
	game_over();
};

void mqa::run()
{
	// ImGui
	Mix_VolumeMusic(50);
	Mix_PlayMusic(mainbgm, -1);
	
	notes_texture.emplace(0, sdl::texture{(std::string(ASSETS_PATH) + "notes/NOTEGUIDE.png").c_str(), rdr});
	notes_texture.emplace(205, sdl::texture{(std::string(ASSETS_PATH) + "notes/NOTE1.png").c_str(), rdr});
	notes_texture.emplace(525, sdl::texture{(std::string(ASSETS_PATH) + "notes/NOTE2.png").c_str(), rdr});
	notes_texture.emplace(190, sdl::texture{(std::string(ASSETS_PATH) + "notes/NOTE3.png").c_str(), rdr});
	notes_texture.emplace(208, sdl::texture{(std::string(ASSETS_PATH) + "notes/NOTE4.png").c_str(), rdr});
	
	SDL_RenderSetVSync(rdr, 1);
	gamereset();

	greeting();
	main_loop();
};


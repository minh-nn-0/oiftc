#include <iostream>

#include "mqa.hpp"

int main()
{
	mqa::game maingame;

	maingame.run_menu();
	std::cout << "============== GAME RUNNING \n";
};


//bool idp_update1(const float& dt, mg::Game& idp)
//{
//	b2World_Step(ps._world, 1.0/30.0, 4);
//	
//	auto& keymap = idp._keymap;
//	auto& input = idp._btnbit;
//	auto& p_body = idp.r_get<tiled::object>("player")->_bodyid;
//	auto& p_rect = idp.r_get<tiled::object>("player")->_rect;
//	
//	// match object pos with physical pos
//	for (auto* object: idp._resource.get_vec<tiled::object>())
//	{
//		auto [bpx, bpy] = b2Body_GetPosition(object->_bodyid);
//		auto& [rx, ry, rw, rh] = object->_rect;
//		rx = bpx / ps._mppx - rw/2.0;
//		ry = bpy / ps._mppx - rw/2.0;
//	};
//
//	idp._btnbit_prev = idp._btnbit;
//	SDL_Event e;
//	while (SDL_PollEvent(&e))
//	{
//		if (e.type == SDL_QUIT) return false;
//		ImGui_ImplSDL2_ProcessEvent(&e);
//		mg::process_input(e, input, keymap);
//	};
//
//	if (idp.btn(4)){b2Body_ApplyLinearImpulseToCenter(p_body, up, true) ;};
//	if (idp.btn(5)){b2Body_ApplyLinearImpulseToCenter(p_body, down, true) ;};
//	if (idp.btn(6)){b2Body_ApplyLinearImpulseToCenter(p_body, left, true) ;};
//	if (idp.btn(7)){b2Body_ApplyLinearImpulseToCenter(p_body, right, true) ;};
//	if (idp.btnp(2)){cam._zoom += 0.2;};
//	if (idp.btnp(3)){cam._zoom -= 0.2;};
//	
//	SDL_GetWindowSize(idp._window, &ws_w, &ws_h);
//	SDL_GetRendererOutputSize(idp._renderer, &cam._dms.w, &cam._dms.h);
//	
//	if (db1) cam.target(p_rect.get_center(), dt);
//	return true;
//
//};

//void idp_draw1(mg::Game& idp)
//{
//	auto* renderer = idp._renderer;
//	auto* map = idp.r_get<tiled::tmap>("map1");
//	auto* ts = idp.r_get<tiled::tset>("idp");
//	auto& [cam_x, cam_y, cam_w, cam_h] = cam._dms;
//	auto [r,g,b] = hexToRGB(map->_bgcolor);
//	
//
//	//SDL_Rect player_src = ts->rect_at(512);
//	//SDL_Rect mob1 = ts->rect_at(m1t);
//	//SDL_Rect mob2 = ts->rect_at(m2t);
//	//SDL_Rect mob3 = ts->rect_at(m3t);
//
//	//SDL_Rect player_dst = *idp.r_get<mg::irect>("player");
//	////set playerpos to center of screen <-------- WRONG
//	////set camera(screen) center to player
//	//
//	//player_dst.x -= cam_x;	
//	//player_dst.y -= cam_y;	
//	//
//	//SDL_Rect mob1_dst = *idp.r_get<mg::irect>("mob1");
//	//mob1_dst.x -= cam_x;
//	//mob1_dst.y -= cam_y;
//	//
//	//SDL_Rect mob2_dst = *idp.r_get<mg::irect>("mob2");
//	//mob2_dst.x -= cam_x;
//	//mob2_dst.y -= cam_y;
//	//
//	//SDL_Rect mob3_dst = *idp.r_get<mg::irect>("mob3");
//	//mob3_dst.x -= cam_x;
//	//mob3_dst.y -= cam_y;
//
//	SDL_SetRenderTarget(renderer, NULL);
//	SDL_SetRendesdl._rendererawColor(renderer, r,g,b,255);
//	SDL_RenderClear(renderer);
//	
//	SDL_RenderSetScale(renderer, cam._zoom, cam._zoom);
//	
//	for (auto& tilelayer: map->_layers_data.get_vec<tiled::tilelayer>())
//	{
//		SDL_Rect src {0, 0, 16*30, 16*20};
//		SDL_Rect dst {static_cast<int>(-cam_x), 
//						static_cast<int>(-cam_y), 
//						16*30, 16*20};
//		SDL_RenderCopy(renderer, **tilelayer->_tex, &src, &dst);
//	};
//
//	for (auto* object: idp._resource.get_vec<tiled::object>())
//	{
//		if (object->_tileid >= 0)
//		{
//			SDL_Rect src = ts->rect_at(object->_tileid);
//			SDL_Rect dst = object->_rect;
//			dst.x -= cam_x;
//			dst.y -= cam_y;
//			auto rot = b2Body_GetRotation(object->_bodyid);
//			SDL_RenderCopyEx(renderer, **ts->_src, &src, &dst, 
//					b2Rot_GetAngle(rot), NULL, 
//					tiled::get_sdlflipflag(object->_flags.to_ulong()));
//		};
//
//	};
//
//	//SDL_RenderCopy(renderer, **ts->_src, &player_src, &player_dst);
//	//SDL_RenderCopy(renderer, **ts->_src, &mob1, &mob1_dst);
//	//SDL_RenderCopy(renderer, **ts->_src, &mob2, &mob2_dst);
//	//SDL_RenderCopy(renderer, **ts->_src, &mob3, &mob3_dst);
//	
//	
//
//	SDL_RenderSetScale(renderer, 1,1);
//	ImGui_ImplSDLRenderer2_NewFrame();
//	ImGui_ImplSDL2_NewFrame();
//	ImGui::NewFrame();
//	{
//		static float f = 0.0f;
//		static int counter = 0;
//
//		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
//
//		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
//		ImGui::Text("%d", map->_width);               // Display some text (you can use a format strings too)
//		SDL_Rect vp;
//		SDL_RenderGetViewport(renderer, &vp);
//		
//		auto player_rect = idp.r_get<tiled::object>("player")->_rect;
//		auto [px,py,pw,ph] = player_rect;
//
//		ImGui::Text("player %d, %d, %d, %d", px, py, pw, ph);
//		ImGui::SliderInt("m1t", &m1t, 0, 700);            // Edit 1 float using a slider from 0.0f to 1.0f
//		ImGui::SliderInt("m2t", &m2t, 0, 700);            // Edit 1 float using a slider from 0.0f to 1.0f
//		ImGui::SliderInt("m3t", &m3t, 0, 700);            // Edit 1 float using a slider from 0.0f to 1.0f
//		ImGui::Text("winw %d, winh %d ", ws_w, ws_h);            // Edit 1 float using a slider from 0.0f to 1.0f
//
//		for (auto* object: idp._resource.get_vec<tiled::object>())
//		{
//			auto body = object->_bodyid;
//			auto rect = object->_rect;
//			b2Vec2 pos = b2Body_GetPosition(body);
//			ImGui::Text("id %d, %s", object->_tileid, object->_name.c_str());
//			ImGui::Indent(2);
//			ImGui::Text("body %f, %f", pos.x, pos.y);
//			ImGui::Text("rect %d, %d, %d, %d", rect.x, rect.y, rect.w, rect.h);
//		};
//
//		if (ImGui::Button("db1")) db1 = !db1; ImGui::SameLine(); ImGui::Text("%d", db1);
//		if (ImGui::Button("db2")) db2 = !db2; ImGui::SameLine(); ImGui::Text("%d", db2);
//		if (ImGui::Button("db3")) db3 = !db3; ImGui::SameLine(); ImGui::Text("%d", db3);
//		if (ImGui::Button("db4")) db4 = !db4; ImGui::SameLine(); ImGui::Text("%d", db4);
//
//		ImGui::End();
//
//		ImGui::Begin("Camera");                          // Create a window called "Hello, world!" and append into it.
//			ImGui::SliderInt("cam_offsetx", &cam._offset.x, -50, 50);            // Edit 1 float using a slider from 0.0f to 1.0f
//			ImGui::SliderInt("cam_offsety", &cam._offset.y, -50, 50);            // Edit 1 float using a slider from 0.0f to 1.0f
//			ImGui::SliderFloat("cam_zoom", &cam._zoom, -10, 10);            // Edit 1 float using a slider from 0.0f to 1.0f
//		ImGui::End();
//    }
//	ImGui::Render();
//	ImGui_ImplSDLRenderer2_Rendesdl._rendererawData(ImGui::GetDrawData(), renderer);
//	SDL_RenderPresent(renderer);
//};

//void setup_idp(mg::Game& idp)
//{
//	auto* map1 = idp.r_add<tiled::tmap>(tiled::tm_from_path("assets/untitled.tmj"), "map1");
//
//	//for (auto& layers: map1->_layers_data.get_vec<tiled::ltile>() | std::views::reverse)
//	//{
//	//	for (auto& i: layers->_tiledata) std::cout << i << ", ";
//
//	//	std::cout << '\n';
//	//};
//
//	for (auto* object: map1->_layers_data.get_vec<tiled::object>())
//	{
//		std::cout << "donaaaaaaaaaaae load object\n";
//		idp.r_add<tiled::object>(*object, object->_name);
//	};
//
//	ps._mppx = 1.0/map1->_tilesize;
//	
//
//	for (auto* object: idp._resource.get_vec<tiled::object>())
//	{
//		auto [rx, ry, rw, rh] = object->_rect;
//		auto [centerx, centery] = object->_rect.get_center() * ps._mppx;
//		std::cout << std::format("id {}, {}\n", object->_name, object->_tileid);
//		std::cout << std::format("p {}, {}, {}, {}\n", rx, ry, rw, rh);
//		std::cout << std::format("c {}, {}\n", centerx, centery);
//	};
//
//
//	// Create physics engine
//
//	b2WorldDef worlddef = b2DefaultWorldDef();
//	worlddef.gravity = {0.0, 10.0};
//	ps._world = b2CreateWorld(&worlddef);
//	
//	b2BodyDef bodydef = b2DefaultBodyDef();
//	for (auto* object: idp._resource.get_vec<tiled::object>())
//	{
//		if (object->_tileid > 0) bodydef.type = b2_dynamicBody;
//		else bodydef.type = b2_staticBody;
//		
//		auto pos = object->_rect.get_center() * ps._mppx;
//		bodydef.position = {pos.x, pos.y};
//			
//		b2BodyId new_body = b2CreateBody(ps._world, &bodydef);
//		
//		auto [boxw, boxh] = object->_rect.get_dms() * ps._mppx;	
//		b2Polygon box = b2MakeBox(boxw/2, boxh/2);
//		b2ShapeDef boxdef = b2DefaultShapeDef();
//		boxdef.friction = 0.5;
//		b2CreatePolygonShape(new_body, &boxdef, &box);
//		
//		object->_bodyid = new_body;
//		ps._bodies.push_back(new_body);
//	};
//	
//	std::cout << "dssssssssssone load object\n";
//};

//int main()
//{
	

//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//    ImGuiIO& io = ImGui::GetIO(); (void)io;
//    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     
//    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      
//    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         
//	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
//	ImGui::StyleColorsDark();
//
//	ImGui_ImplSDL2_InitForSDLRenderer(idp._window, idp._renderer);
//	ImGui_ImplSDLRenderer2_Init(idp._renderer);
	

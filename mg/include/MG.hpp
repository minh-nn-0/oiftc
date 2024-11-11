#ifndef M_GAME_H
#define M_GAME_H

#include <functional>
#include <bitset>
#include <vector>

#include <imgui.h>
#include <imgui_impl_sdlrenderer2.h>
#include <imgui_impl_sdl2.h>

#include <glm/glm.hpp>

#include <mSDL.hpp>
#include <mtiled.hpp>
#include <mcamera.hpp>
#include <mphysics.hpp>
#include <msprite.hpp>
#include <minput.hpp>
#include <manimation.hpp>
#include <mutilities.hpp>
#include <geometry.hpp>


namespace mg
{
	struct time_tracker
	{
		int prev, curr;
		int elapsed() {return curr-prev;};
	};
	
	struct timer
	{
		float _limit;
		float _elapsed;
		
		void reset() {_elapsed = 0;};
		void update(const float& dt)
		{
			if (running()) _elapsed += dt;
		};
		bool running() {return _elapsed < _limit;}
	};
	// contains a set of rules, flags
	struct game
	{
		game();

		~game();
		
		bool _running = true;

		//# FLAGS
		std::bitset<64> _flags;
	};

	constexpr bool name_is_empty(auto&& x) { return x._name.empty(); };

	inline void init_ImGui(SDL_Window* wdw, SDL_Renderer* rdr)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         
		ImGui::StyleColorsDark();
		ImGui_ImplSDL2_InitForSDLRenderer(wdw, rdr);
		ImGui_ImplSDLRenderer2_Init(rdr);
	};

}
#endif

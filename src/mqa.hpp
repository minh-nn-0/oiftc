#ifndef M_QA_HPP
#define M_QA_HPP

#include <utility>
#include <Beaver/core.hpp>
template<typename T> 
using state = std::pair<T,T>;

template<typename T>
bool state_is(const state<T> s, T&& pred) { return s.first == pred; };

template<typename T>
bool state_just_change(const state<T> s, T&& pred) { return s.first == pred && !(s.second == pred); };

template<typename T>
void state_set(state<T>& s, T&& v) {s.first = v;} 

template<typename T>
void state_reserve(state<T>& s) {s.second = s.first;};

struct timer
{
	float _limit;
	float _elapsed;
	
	void reset() {_elapsed = 0;};
	void update(const float& dt)
	{
		if (running()) _elapsed += dt / 60;
	};
	bool running() {return _elapsed < _limit;}
};


namespace mqa
{
	void run();
}


#endif

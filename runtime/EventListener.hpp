#pragma once
#include <memory>
#include <atomic>

namespace scratch {
	class EventListener
	{
	public:
		std::atomic<bool> is_broadcasted_play_snd_select;
		std::atomic<bool> is_broadcasted_show_melee_menu;
		std::atomic<bool> is_broadcasted_show_title_screen;
	public:
		inline EventListener() {
			is_broadcasted_play_snd_select.store(false);
			is_broadcasted_show_melee_menu.store(false);
			is_broadcasted_show_title_screen.store(false);
		}
		inline void broadcast_event_play_snd_select() {
			is_broadcasted_play_snd_select.store(true);
		}
		inline void broadcast_event_show_melee_menu() {
			is_broadcasted_show_melee_menu.store(true);
		}
		inline void broadcast_event_show_title_screen() {
			is_broadcasted_show_title_screen.store(true);
		}
		void tick();
	};
};


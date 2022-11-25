#pragma once
#include <iostream>
#include <mutex>
#include <deque>
#include <memory>
#include <unordered_map>

#include "Backend.hpp"
#include "Target.hpp"
#include "Sound.hpp"
#include "EventListener.hpp"
#include "Utils.hpp"

namespace scratch {
	class App {
	private:
		const unsigned char* m_key_buffer;
		unsigned int m_click_state;

		std::chrono::system_clock::time_point last_render;

		std::mutex renderer_lock;
		std::mutex mouse_lock;
		std::shared_ptr<scratch::backend::Renderer> m_renderer;
		std::shared_ptr<scratch::EventListener> m_event_listener;
		
	public:
		int m_mouse_x;
		int m_mouse_y;

		scratch::utils::SortedHashMap<std::string, std::shared_ptr<scratch::Target>> m_targets;
		App();

		void start();
		void poll_input();

		std::shared_ptr<scratch::Target> stage();
		std::shared_ptr<scratch::EventListener> event_listener();
		std::shared_ptr<scratch::backend::Renderer> renderer();

		void request_render();

		bool mouse_touching(const scratch::Target*);
		bool mouse_down();
		void play_sound(const scratch::Sound&);

		constexpr inline int screen_x() {
			return 480;
		}
		
		constexpr inline int screen_y() {
			return 360;
		}
	};
	
	/* Acquire the running app instance. */
	std::shared_ptr<scratch::App> app();
};

class unimplemented_errror : public std::exception {};
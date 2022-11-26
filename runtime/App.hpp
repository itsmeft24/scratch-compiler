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
		std::chrono::system_clock::time_point m_last_render;
		std::mutex m_renderer_lock;
		std::shared_ptr<scratch::backend::Renderer> m_renderer;
		std::shared_ptr<scratch::EventListener> m_event_listener;
	public:
		std::atomic<int> m_mouse_x;
		std::atomic<int> m_mouse_y;
		// To Do: Lock this!
		scratch::utils::SortedHashMap<std::string, std::shared_ptr<scratch::Target>> m_targets;

		App();
		App(const App&) = delete;
		App& operator=(const App&) = delete;

		// Getters.
		std::shared_ptr<scratch::Target> stage();
		std::shared_ptr<scratch::EventListener> event_listener();
		std::shared_ptr<scratch::backend::Renderer> renderer();
		constexpr inline int screen_x() {
			return 480;
		}
		constexpr inline int screen_y() {
			return 360;
		}

		// Functions that sprites may call into.
		template <class T> requires std::derived_from<T, scratch::Target> void make_clone(const std::string& name) {
			const auto& clone = std::make_shared<T>();
			this->m_targets.insert(name, clone);
			std::thread handle([clone] {clone->green_flag(); });
			handle.detach();
		}

		void poll_input();
		bool mouse_touching(const scratch::Target*);
		bool mouse_down();
		void play_sound(const scratch::Sound&);

		// Functions called by the outside world...
		void start();
		void request_render();
	};
	
	// Acquires the running app instance.
	std::shared_ptr<scratch::App> app();
};
#pragma once
#include <iostream>
#include <mutex>
#include <vector>
#include "Costume.hpp"

namespace scratch {
	class Target
	{
	private:
		std::string m_name;
		int m_costume_index;
		bool m_is_visible;
		double m_rotation;
		double m_size;
		int m_trans_x;
		int m_trans_y;
		bool was_clicked;
		// To Do: Implement these!
		// bool m_draggable;
		// RotationStyle m_rotation_style;
		mutable std::mutex mutex;
	protected:
		std::vector<scratch::Costume> costumes;
	public:
		Target();
		Target(const Target&) = delete;
		Target& operator=(const Target&) = delete;
		void setup(const std::string& str, int trans_x, int trans_y, double size, bool is_visible);
		void next_costume();
		void set_visible(bool is_visible);
		void set_size(double size);
		void set_rotation(double rot);
		void render(std::shared_ptr<scratch::backend::Renderer> renderer);
		SDL_Rect get_rect() const;
		virtual void switch_costume(int index);
		virtual void green_flag();
		virtual void clicked();
		virtual void clone_start();
		void tick();
		constexpr inline double get_size() const {
			return m_size;
		}
		constexpr inline double get_rotation() const {
			return m_rotation;
		}
		constexpr inline std::pair<double, double> get_rotation_axis() const {
			return costumes[m_costume_index].get_rotation_axis();
		}
	};
};


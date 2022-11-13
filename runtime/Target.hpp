#pragma once
#include <vector>
#include "Costume.hpp"
namespace scratch {
	class Target
	{
	private:
		int m_costume_index;
		bool m_is_visible;
		double m_rotation;
		int m_size;
		int m_trans_x;
		int m_trans_y;
	protected:
		std::vector<scratch::Costume> costumes;

	public:
		Target();
		Target(const Target&) = delete;
		Target& operator=(const Target&) = delete;
		void switch_costume(int index);
		void next_costume();
		void set_visible(bool is_visible);
		void set_rotation(double rot);
		void render();
		virtual void green_flag();
		virtual void clicked();
	};
};


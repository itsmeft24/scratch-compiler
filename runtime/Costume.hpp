#pragma once
#include <string>
#include "Backend.hpp"

namespace scratch {
	class Costume
	{
	private:
		double m_rotation_center_x;
		double m_rotation_center_y;
		std::unique_ptr<scratch::backend::Image> m_backing_image;
	public:
		Costume(const std::string& file_path, double rotation_center_x, double rotation_center_y);
		Costume(const Costume&) = delete;
		Costume(Costume&&) = default;
		Costume& operator= (const Costume&) = delete;
		SDL_Rect get_rect(double x, double y, double size) const;
		void render(std::shared_ptr<scratch::backend::Renderer> renderer, double x, double y, double rotation, double size) const;
		
		constexpr inline std::pair<int, int> get_size() const {
			return { m_backing_image->width() , m_backing_image->height() };
		}
		constexpr inline std::pair<double, double> get_rotation_axis() const {
			return { m_rotation_center_x , m_rotation_center_y };
		}
	};
};


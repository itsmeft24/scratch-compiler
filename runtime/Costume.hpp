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
		void render(double x, double y, double rotation, double size) const;
	};
};


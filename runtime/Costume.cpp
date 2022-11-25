
#include "App.hpp"
#include "Costume.hpp"

scratch::Costume::Costume(const std::string& file_path, double rotation_center_x, double rotation_center_y)
{
	m_rotation_center_x = rotation_center_x;
	m_rotation_center_y = rotation_center_y;
	m_backing_image = std::make_unique<scratch::backend::Image>(file_path);
}

SDL_Rect scratch::Costume::get_rect(double x, double y, double size) const
{
	return {
		static_cast<int>(std::round(x + scratch::app()->screen_x() / 2.0f - m_rotation_center_x * size)),
		static_cast<int>(std::round(-y + scratch::app()->screen_y() / 2.0f - m_rotation_center_y * size)),
		static_cast<int>(std::round(this->m_backing_image->width() * size)),
		static_cast<int>(std::round(this->m_backing_image->height() * size)),
	};
}

void scratch::Costume::render(std::shared_ptr<scratch::backend::Renderer> renderer, double x, double y, double rotation, double size) const
{
	SDL_Rect source{
		0,
		0,
		this->m_backing_image->width(),
		this->m_backing_image->height()
	};

	SDL_Rect dest = this->get_rect(x, y, size);
	
	SDL_Point axis{
		static_cast<int>(std::round(this->m_rotation_center_x)),
		static_cast<int>(std::round(this->m_rotation_center_y))
	};

	renderer->blit(this->m_backing_image, &source, &dest, rotation, &axis);
}


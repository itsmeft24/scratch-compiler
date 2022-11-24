
#include "App.hpp"
#include "Costume.hpp"

scratch::Costume::Costume(const std::string& file_path, double rotation_center_x, double rotation_center_y)
{
	m_rotation_center_x = rotation_center_x;
	m_rotation_center_y = rotation_center_y;
	m_backing_image = std::make_unique<scratch::backend::Image>(file_path);
}

void scratch::Costume::render(double x, double y, double rotation, double size) const
{
	SDL_Rect source{
		0,
		0,
		this->m_backing_image->width(),
		this->m_backing_image->height()
	};
	SDL_Rect dest{
		x + scratch::app()->screen_x() / 2.0f - this->m_backing_image->width() * size / 2.0f,
		y + scratch::app()->screen_y() / 2.0f - this->m_backing_image->height() * size / 2.0f,
		this->m_backing_image->width() * size,
		this->m_backing_image->height() * size
	};
	
	SDL_Point axis{
		this->m_rotation_center_x,
		this->m_rotation_center_y
	};

	auto[lock, renderer] = scratch::app()->renderer();

	renderer->start_frame();

	renderer->blit(*this->m_backing_image.get(), &source, &dest, rotation, &axis);

	renderer->end_frame();
}

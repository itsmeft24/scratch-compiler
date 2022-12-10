
#include "App.hpp"
#include "Costume.hpp"

scratch::Costume::Costume(const std::string& file_path, int bitmap_resolution, double rotation_center_x, double rotation_center_y)
{
	m_bitmap_resolution = bitmap_resolution;
	m_rotation_center_x = rotation_center_x;
	m_rotation_center_y = rotation_center_y;
	m_backing_image = std::make_unique<scratch::backend::Image>(file_path);
}

SDL_Rect scratch::Costume::get_rect(double x, double y, double size) const
{
	return {
		static_cast<int>(std::round(x + scratch::backend::Renderer::get_logical_size().first / 2.0f - m_rotation_center_x * size / this->m_bitmap_resolution)),
		static_cast<int>(std::round(-y + scratch::backend::Renderer::get_logical_size().second / 2.0f - m_rotation_center_y * size / this->m_bitmap_resolution)),
		static_cast<int>(std::round(this->m_backing_image->width() * size / this->m_bitmap_resolution)),
		static_cast<int>(std::round(this->m_backing_image->height() * size / this->m_bitmap_resolution)),
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
		static_cast<int>(std::round(this->m_rotation_center_x * size / this->m_bitmap_resolution)),
		static_cast<int>(std::round(this->m_rotation_center_y * size / this->m_bitmap_resolution))
	};

	renderer->blit(this->m_backing_image, &source, &dest, rotation, &axis);
}


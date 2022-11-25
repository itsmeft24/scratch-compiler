#include "Target.hpp"
#include "App.hpp"

scratch::Target::Target()
{
	m_costume_index = 0;
	m_is_visible = false;
	m_rotation = 0.0;
	m_size = 0;
	m_trans_x = 0;
	m_trans_y = 0;
}

void scratch::Target::switch_costume(int index)
{
	if (m_costume_index < costumes.size()) {
		m_costume_index = index;
	}
	scratch::app()->request_render();
}

void scratch::Target::setup(const std::string& str, int trans_x, int trans_y, double size, bool is_visible)
{
	m_name = str;
	m_trans_x = trans_x;
	m_trans_y = trans_y;
	m_size = size;
	m_is_visible = is_visible;
}

void scratch::Target::next_costume()
{
	if (m_costume_index < costumes.size()) {
		m_costume_index++;
	}
	scratch::app()->request_render();
}

void scratch::Target::set_visible(bool is_visible)
{
	m_is_visible = is_visible;
	scratch::app()->request_render();
}

void scratch::Target::set_size(double size)
{
	m_size = size;
	scratch::app()->request_render();
}

void scratch::Target::set_rotation(double rot)
{
	m_rotation += rot;
	if (m_rotation > 360.0f) {
		m_rotation -= 360.0f;
	}
	scratch::app()->request_render();
}

void scratch::Target::render(std::shared_ptr<scratch::backend::Renderer> renderer)
{
	if (m_costume_index < costumes.size() && m_is_visible) {
		const auto& costume = costumes.at(m_costume_index);
		costume.render(renderer, m_trans_x, m_trans_y, m_rotation, m_size);
	}
}

SDL_Rect scratch::Target::get_rect() const
{
	return costumes[m_costume_index].get_rect(m_trans_x, m_trans_y, m_size);
}

void scratch::Target::green_flag()
{
}

void scratch::Target::clicked()
{
}

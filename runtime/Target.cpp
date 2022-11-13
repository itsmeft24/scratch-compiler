#include "Target.hpp"

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
	this->render();
}

void scratch::Target::next_costume()
{
	if (m_costume_index < costumes.size()) {
		m_costume_index++;
	}
	this->render();
}

void scratch::Target::set_visible(bool is_visible)
{
	m_is_visible = is_visible;
}

void scratch::Target::set_rotation(double rot)
{
	m_rotation = rot;
}

void scratch::Target::render()
{
	if (m_costume_index < costumes.size() && m_is_visible) {
		const auto& costume = costumes.at(m_costume_index);
		costume.render(m_trans_x, m_trans_y, m_rotation, m_size);
	}
}

void scratch::Target::green_flag()
{
}

void scratch::Target::clicked()
{
}

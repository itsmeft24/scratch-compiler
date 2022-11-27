#include "Target.hpp"
#include "App.hpp"

scratch::Target::Target()
{
	m_costume_index = 0;
	m_is_visible = false;
	m_rotation = 0.0;
	m_size = 1.0f;
	m_trans_x = 0;
	m_trans_y = 0;
}

void scratch::Target::switch_costume(int index)
{
	mutex.lock();
	if (m_costume_index < costumes.size()) {
		m_costume_index = index;
	}
	mutex.unlock();
	scratch::app()->sync_next_frame();
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
	mutex.lock();
	if (m_costume_index < costumes.size()) {
		m_costume_index++;
	}
	mutex.unlock();
	scratch::app()->sync_next_frame();
}

void scratch::Target::set_visible(bool is_visible)
{
	mutex.lock();
	m_is_visible = is_visible;
	mutex.unlock();
	scratch::app()->sync_next_frame();
}

void scratch::Target::set_size(double size)
{
	mutex.lock();
	m_size = size;
	mutex.unlock();
	scratch::app()->sync_next_frame();
}

void scratch::Target::set_rotation(double rot)
{
	mutex.lock();
	m_rotation += rot;
	if (m_rotation > 360.0f) {
		m_rotation -= 360.0f;
	}
	mutex.unlock();
	scratch::app()->sync_next_frame();
}

void scratch::Target::render(std::shared_ptr<scratch::backend::Renderer> renderer)
{
	std::scoped_lock<std::mutex> lock(mutex);
	if (m_costume_index < costumes.size() && m_is_visible) {
		const auto& costume = costumes.at(m_costume_index);
		costume.render(renderer, m_trans_x, m_trans_y, m_rotation, m_size);
	}
}

SDL_Rect scratch::Target::get_rect() const
{
	std::scoped_lock<std::mutex> lock(mutex);
	return costumes[m_costume_index].get_rect(m_trans_x, m_trans_y, m_size);
}

void scratch::Target::green_flag()
{
}

void scratch::Target::clicked()
{
}

void scratch::Target::clone_start()
{
}

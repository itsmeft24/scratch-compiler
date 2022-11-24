#include <thread>
#include <functional>
#include "App.hpp"


extern std::shared_ptr<scratch::App> g_main_app;

scratch::App::App()
{
    m_key_buffer = nullptr;
    m_click_state = 0;
    m_mouse_x = 0;
    m_mouse_y = 0;
    m_renderer = std::make_shared<scratch::backend::Renderer>();
    m_event_listener = std::make_shared<scratch::EventListener>();
}

void scratch::App::start()
{
    for (const auto& target : m_targets) {
        std::thread handle([target] { target.second->green_flag(); });
        handle.detach();
    }
}

std::shared_ptr<scratch::Target> scratch::App::stage()
{
    return m_targets.at("Stage");
}

std::shared_ptr<scratch::EventListener> scratch::App::event_listener()
{
    return m_event_listener;
}

std::pair<std::scoped_lock<std::mutex>, std::shared_ptr<scratch::backend::Renderer>> scratch::App::renderer()
{
    return std::pair<std::scoped_lock<std::mutex>, std::shared_ptr<scratch::backend::Renderer>>(renderer_lock, m_renderer);
}

void scratch::App::poll_input()
{
    m_click_state = SDL_GetMouseState(&m_mouse_x, &m_mouse_y);
    m_key_buffer = SDL_GetKeyboardState(nullptr);
}

bool scratch::App::mouse_touching(const scratch::Target*)
{
    return false;
}

bool scratch::App::mouse_down()
{
    return (m_click_state & SDL_BUTTON_LMASK) > 0 || (m_click_state & SDL_BUTTON_RMASK) > 0;
}

void scratch::App::play_sound(const scratch::Sound& sound)
{
    Mix_PlayChannel(-1, sound.get_chunk(), 0);
}

std::shared_ptr<scratch::App> scratch::app()
{
    return g_main_app;
}

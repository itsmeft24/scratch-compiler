#include <numbers>
#include <thread>
#include <functional>
#include "App.hpp"
#include "Utils.hpp"

using namespace std::chrono_literals;

extern std::shared_ptr<scratch::App> g_app;

scratch::App::App()
{
    m_last_render = std::chrono::system_clock::now();
    m_key_buffer = nullptr;
    m_click_state = 0;
    m_mouse_x = 0;
    m_mouse_y = 0;
    m_renderer = std::make_shared<scratch::backend::Renderer>();
    m_event_listener = std::make_shared<scratch::EventListener>();
}

std::shared_ptr<scratch::Target> scratch::App::stage()
{
    return m_targets.at("Stage");
}

std::shared_ptr<scratch::EventListener> scratch::App::event_listener()
{
    return m_event_listener;
}

std::shared_ptr<scratch::backend::Renderer> scratch::App::renderer()
{
    return m_renderer;
}

void scratch::App::poll_input()
{
    int mouse_x = 0, mouse_y = 0;
    m_click_state = SDL_GetMouseState(&mouse_x, &mouse_y);
    m_mouse_x.store(mouse_x);
    m_mouse_y.store(mouse_y);
}

bool scratch::App::mouse_touching(const scratch::Target* target)
{
    double angle = target->get_rotation() * std::numbers::pi / 180.0f;
    auto [rot_x, rot_y] = target->get_rotation_axis();
    SDL_Rect rect = target->get_rect();

    if (target->get_rotation() == 0) {
        SDL_Point mouse{m_mouse_x, m_mouse_y};
        return SDL_PointInRect(&mouse, &rect);
    }

    return m_mouse_x * std::cos(angle) - m_mouse_y * std::sin(angle) + rot_x >= rect.x
        && m_mouse_x * std::cos(angle) - m_mouse_y * std::sin(angle) + rot_x <= rect.x + rect.w
        && m_mouse_x * std::sin(angle) + m_mouse_y * std::cos(angle) + rot_y >= rect.y
        && m_mouse_x * std::sin(angle) + m_mouse_y * std::cos(angle) + rot_y <= rect.y + rect.h;
}

bool scratch::App::mouse_down()
{
    return (m_click_state & SDL_BUTTON_LMASK) > 0 || (m_click_state & SDL_BUTTON_RMASK) > 0;
}

void scratch::App::play_sound(const scratch::Sound& sound)
{
    Mix_PlayChannel(-1, sound.get_chunk(), 0);
}

void scratch::App::start()
{
    m_key_buffer = SDL_GetKeyboardState(nullptr);

    for (size_t x = 0; x < m_targets.size(); x++) {
        auto& target = m_targets.at_index(x);
        std::thread handle([target] {target->green_flag(); });
        handle.detach();
    }
}

void scratch::App::request_render()
{
    /*
    std::scoped_lock<std::mutex> lock(renderer_lock);
    std::chrono::system_clock::duration elapsed = std::chrono::system_clock::now() - last_render;
    if (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed) >= 33.3333333ms) {
        m_renderer->start_frame();
        for (size_t x = 0; x < m_targets.size(); x++) {
            m_targets.at_index(x)->render(m_renderer);
        }
        m_renderer->end_frame();
        last_render = std::chrono::system_clock::now();
    }
    */
    std::scoped_lock<std::mutex> lock(m_renderer_lock);
    m_renderer->start_frame();
    for (size_t x = 0; x < m_targets.size(); x++) {
        m_targets.at_index(x)->render(m_renderer);
    }

    std::chrono::system_clock::duration elapsed = std::chrono::system_clock::now() - m_last_render;
    const auto& elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
    if (elapsed_ms <= 33.3333333ms) {
        SDL_Delay(static_cast<unsigned int>(std::floor((33.3333333ms - elapsed_ms).count())));
    }

    m_renderer->end_frame();
    m_last_render = std::chrono::system_clock::now();
}

std::shared_ptr<scratch::App> scratch::app()
{
    return g_app;
}


#include <numbers>
#include <thread>
#include <functional>
#include "App.hpp"
#include "Utils.hpp"

using namespace std::chrono_literals;

// constexpr auto FRAME_TIME = 33.3333333ms;
constexpr auto FRAME_TIME = 16.6666666ms;

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
    frame_syncer.store(0);
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

    for (size_t x = 0; x < m_targets.size(); x++) {
        auto& target = m_targets.at_index(x);
        std::thread handle([target] {target->tick(); });
        handle.detach();
    }
}

void scratch::App::sync_next_frame()
{
    const auto& old = frame_syncer.load();
    frame_syncer.wait(old);
}

bool scratch::App::mouse_touching(const scratch::Target* target)
{
    double angle = target->get_rotation() * std::numbers::pi / 180.0f;
    auto [rot_x, rot_y] = target->get_rotation_axis();

    auto [x_scale, y_scale] = scratch::backend::Renderer::get_logical_scale();
    SDL_Rect rect = target->get_rect();
    rot_x *= target->get_size() * x_scale;
    rot_y *= target->get_size() * y_scale;

    return (m_mouse_x * x_scale - rot_x) * std::cos(angle) - (m_mouse_y * y_scale - rot_y) * std::sin(angle) + rot_x >= rect.x
        && (m_mouse_x * x_scale - rot_x) * std::cos(angle) - (m_mouse_y * y_scale - rot_y) * std::sin(angle) + rot_x <= rect.x + rect.w
        && (m_mouse_x * x_scale - rot_x) * std::sin(angle) + (m_mouse_y * y_scale - rot_y) * std::cos(angle) + rot_y >= rect.y
        && (m_mouse_x * x_scale - rot_x) * std::sin(angle) + (m_mouse_y * y_scale - rot_y) * std::cos(angle) + rot_y <= rect.y + rect.h;
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

void scratch::App::render_frame()
{
    m_renderer->start_frame();
    for (size_t x = 0; x < m_targets.size(); x++) {
        m_targets.at_index(x)->render(m_renderer);
    }

    std::chrono::system_clock::duration elapsed = std::chrono::system_clock::now() - m_last_render;
    const auto& elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
    if (elapsed_ms <= FRAME_TIME) {
        SDL_Delay(static_cast<unsigned int>(std::floor((FRAME_TIME - elapsed_ms).count())));
    }

    m_renderer->end_frame();
    m_last_render = std::chrono::system_clock::now();

    frame_syncer.fetch_xor(1);
    frame_syncer.notify_all();
}

std::shared_ptr<scratch::App> scratch::app()
{
    return g_app;
}


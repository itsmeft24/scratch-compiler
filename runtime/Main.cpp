#include <iostream>
#include <SDL2/SDL.h>
#include "App.hpp"

std::shared_ptr<scratch::App> g_app = std::make_unique<scratch::App>();

#include "sprites/stage.hxx"
#include "sprites/main_menu_btn00_melee_frame_u.hxx"
#include "sprites/main_menu_btn3_00_other_frame_u.hxx"
#include "sprites/main_menu_btn3_00_colle_frame_u.hxx"
#include "sprites/main_menu_btn3_00_splits_frame_u.hxx"
#include "sprites/main_menu_btn3_00_online_frame_u.hxx"
#include "sprites/main_menu_btn3_00_eshop_frame_u.hxx"
#include "sprites/loading_pic_01_u.hxx"
#include "sprites/loading_pic_03_u.hxx"
#include "sprites/sidebar.hxx"
#include "sprites/p3_other_top_btn3_06_circle_00_s2.hxx"
#include "sprites/melee_top_preview_normal_u.hxx"
#include "sprites/melee_icon1.hxx"
#include "sprites/melee_icon2.hxx"
#include "sprites/melee_icon3.hxx"
#include "sprites/melee_icon4.hxx"
#include "sprites/melee_icon5.hxx"
#include "sprites/back_btn.hxx"
#include "sprites/title_screen.hxx"

extern "C" int main(int argc, char* argv[])
{
    g_app->m_targets = {
        { "Stage", dynamic_pointer_cast<scratch::Target>(std::make_shared<Stage>())},
        { "loading_pic_03_u", dynamic_pointer_cast<scratch::Target>(std::make_shared<loading_pic_03_u>())},
        { "loading_pic_01_u", dynamic_pointer_cast<scratch::Target>(std::make_shared<loading_pic_01_u>())},
        { "main_menu_btn3_00_splits_frame_u", dynamic_pointer_cast<scratch::Target>(std::make_shared<main_menu_btn3_00_splits_frame_u>())},
        { "main_menu_btn3_00_other_frame_u", dynamic_pointer_cast<scratch::Target>(std::make_shared<main_menu_btn3_00_other_frame_u>())},
        { "main_menu_btn3_00_colle_frame_u", dynamic_pointer_cast<scratch::Target>(std::make_shared<main_menu_btn3_00_colle_frame_u>())},
        { "main_menu_btn3_00_online_frame_u", dynamic_pointer_cast<scratch::Target>(std::make_shared<main_menu_btn3_00_online_frame_u>())},
        { "main_menu_btn3_00_eshop_frame_u", dynamic_pointer_cast<scratch::Target>(std::make_shared<main_menu_btn3_00_eshop_frame_u>())},
        { "melee_top_preview_normal_u", dynamic_pointer_cast<scratch::Target>(std::make_shared<melee_top_preview_normal_u>())},
        { "sidebar", dynamic_pointer_cast<scratch::Target>(std::make_shared<sidebar>())},
        { "melee_icon4", dynamic_pointer_cast<scratch::Target>(std::make_shared<melee_icon4>())},
        { "melee_icon5", dynamic_pointer_cast<scratch::Target>(std::make_shared<melee_icon5>())},
        { "melee_icon3", dynamic_pointer_cast<scratch::Target>(std::make_shared<melee_icon3>())},
        { "melee_icon2", dynamic_pointer_cast<scratch::Target>(std::make_shared<melee_icon2>())},
        { "melee_icon1", dynamic_pointer_cast<scratch::Target>(std::make_shared<melee_icon1>())},
        { "back_btn", dynamic_pointer_cast<scratch::Target>(std::make_shared<back_btn>())},
        { "title_screen", dynamic_pointer_cast<scratch::Target>(std::make_shared<title_screen>())},
        { "main_menu_btn00_melee_frame_u", dynamic_pointer_cast<scratch::Target>(std::make_shared<main_menu_btn00_melee_frame_u>())},
        { "p3_other_top_btn3_06_circle_00_s2", dynamic_pointer_cast<scratch::Target>(std::make_shared<p3_other_top_btn3_06_circle_00_s2>())},
    };
    g_app->start();
    SDL_Event event;
    while (1) {
        g_app->poll_input();

        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        }

        g_app->event_listener()->tick();

        g_app->render_frame();
    }
    return 0;
}
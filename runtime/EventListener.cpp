#include "EventListener.hpp"

#include "App.hpp"

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

void scratch::EventListener::tick()
{
	if (this->is_broadcasted_play_snd_select.load()) {
		std::thread handle([] { dynamic_pointer_cast<Stage>(scratch::app()->m_targets.at("Stage"))->recieve_play_snd_select(); });
		handle.detach();
		this->is_broadcasted_play_snd_select.store(false);
	}
	if (this->is_broadcasted_show_melee_menu.load()) {
		std::thread handle1([] { dynamic_pointer_cast<Stage>(scratch::app()->m_targets.at("Stage"))->recieve_show_melee_menu(); });
		handle1.detach();
		this->is_broadcasted_show_melee_menu.store(false);
	}
	if (this->is_broadcasted_show_title_screen.load()) {
		std::thread handle2([] { dynamic_pointer_cast<loading_pic_03_u>(scratch::app()->m_targets.at("loading_pic_03_u"))->recieve_show_title_screen(); });
		handle2.detach();
		std::thread handle3([] { dynamic_pointer_cast<title_screen>(scratch::app()->m_targets.at("title_screen"))->recieve_show_title_screen(); });
		handle3.detach();
		this->is_broadcasted_show_title_screen.store(false);
	}
}

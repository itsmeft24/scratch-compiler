
#include "Backend.hpp"
#include <SDL2/SDL_image.h>
#include "App.hpp"

namespace scratch {
	namespace backend {
		
		Image::Image(const std::string& file_path)
		{
			m_backing_texture = IMG_LoadTexture(scratch::app()->renderer()->get_backing_renderer(), file_path.c_str());
			SDL_QueryTexture(m_backing_texture, nullptr, nullptr, &m_width, &m_height);
		}

		Image::~Image() {
			SDL_DestroyTexture(m_backing_texture);
			m_width = 0;
			m_height = 0;
		}

		SDL_Texture* Image::get_backing_texture() const {
			return m_backing_texture;
		}

		bool Image::update_texture(SDL_Rect rect, void* data, int pitch) {
			return SDL_UpdateTexture(m_backing_texture, &rect, data, pitch) == 0;
		}

		Renderer::Renderer() {
			auto [logical_x, logical_y] = get_logical_size();
			auto [resolution_x, resolution_y] = get_screen_resolution();
			SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
			IMG_Init(IMG_INIT_PNG);
			Mix_Init(MIX_INIT_MP3);
			Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048);
			m_window = SDL_CreateWindow("Scratch Codegen Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, resolution_x, resolution_y, 0);
			m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
			m_screen_surface = SDL_GetWindowSurface(m_window);
			SDL_RenderSetLogicalSize(m_renderer, logical_x, logical_y);
		}
		
		Renderer::~Renderer() {
			SDL_DestroyWindow(m_window);
			SDL_DestroyRenderer(m_renderer);
		}
		
		void Renderer::start_frame() {
			SDL_RenderClear(m_renderer);
		}

		void Renderer::end_frame() {
			SDL_RenderPresent(m_renderer);
		}

		void Renderer::blit(const std::unique_ptr<Image>& texture, const SDL_Rect* src_rect, SDL_Rect* dst_rect) {
			SDL_RenderCopy(m_renderer, texture->get_backing_texture(), src_rect, dst_rect);
		}

		void Renderer::blit(const std::unique_ptr<Image>& texture, const SDL_Rect* src_rect, SDL_Rect* dst_rect, double rotation, SDL_Point* axis) {
			SDL_RenderCopyEx(m_renderer, texture->get_backing_texture(), src_rect, dst_rect, rotation, axis, SDL_FLIP_NONE);
		}

		void Renderer::blit_flipped(const std::unique_ptr<Image>& texture, const SDL_Rect* src_rect, SDL_Rect* dst_rect) {
			SDL_RenderCopyEx(m_renderer, texture->get_backing_texture(), src_rect, dst_rect, 0.0f, nullptr, SDL_FLIP_HORIZONTAL);
		}
		
		SDL_Renderer* Renderer::get_backing_renderer() {
			return m_renderer;
		}
	};
};
#pragma once

#include <iostream>
#include <optional>
#include <SDL2/SDL.h>

namespace scratch {
	namespace backend {
		
		class Image {
		private:
			SDL_Texture* m_backing_texture;
			int m_width;
			int m_height;
		public:
			
			Image(const std::string& file_path);
			
			Image(SDL_Texture* backing_texture, int width, int height) : m_backing_texture(backing_texture), m_width(width), m_height(height) {}

			~Image();

			Image(const Image&) = delete;

			Image& operator= (const Image&) = delete;

			SDL_Texture* get_backing_texture() const;

			bool update_texture(SDL_Rect rect, void* data, int pitch);

			constexpr int width() const {
				return m_width;
			}

			constexpr int height() const {
				return m_height;
			}
		};

		class Renderer
		{
		private:
			SDL_Renderer* m_renderer;
			SDL_Window* m_window;
			SDL_Surface* m_screen_surface;
			float m_desired_fps;
			uint64_t m_performance_counter;
		public:
			Renderer();

			Renderer(const Renderer&) = delete;

			Renderer& operator=(const Renderer&) = delete;
			
			~Renderer();

			SDL_Renderer* get_backing_renderer();
			
			void start_frame();

			void end_frame();
			
			void blit(const std::unique_ptr<Image>& texture, const SDL_Rect* src_rect, SDL_Rect* dst_rect);

			void blit(const std::unique_ptr<Image>& texture, const SDL_Rect* src_rect, SDL_Rect* dst_rect, double rotation, SDL_Point* axis);

			void blit_flipped(const std::unique_ptr<Image>& texture, const SDL_Rect* src_rect, SDL_Rect* dst_rect);
		};
	};
};
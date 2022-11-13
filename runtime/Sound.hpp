#pragma once

#include <string>
#include <SDL2/SDL_mixer.h>

namespace scratch {
	class Sound
	{
	private:
		Mix_Chunk* m_chunk;
	public:
		Sound(const std::string& file_path);
		Sound(const Sound&) = delete;
		Sound& operator= (const Sound&) = delete;
		constexpr inline Mix_Chunk* get_chunk() const{
			return m_chunk;
		}
		~Sound();
	};
};


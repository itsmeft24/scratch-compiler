#include <iostream>
#include "Sound.hpp"

scratch::Sound::Sound(const std::string& file_path)
{
	m_chunk = Mix_LoadWAV(file_path.c_str());
	if (m_chunk == nullptr) {
		std::cout << SDL_GetError();
	}
}

scratch::Sound::~Sound()
{
	Mix_FreeChunk(m_chunk);
}

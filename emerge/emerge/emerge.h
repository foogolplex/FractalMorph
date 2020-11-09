#include <SDL.h>
#pragma once
class emerge
{
public:
	emerge();
	~emerge();

	int init_sdl();
	int game_loop();
	void clean();
private:
	SDL_Window* window;
	void load_media();
};


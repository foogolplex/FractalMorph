#include "emerge.h"

emerge::emerge()
{
	window = NULL;
}
emerge::~emerge()
{}
int emerge::init_sdl()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		SDL_Log("SDL failed to initialize: %s", SDL_GetError());
		return 1;
	}
	
	window = SDL_CreateWindow(
		"emerge",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_RESIZABLE
	);
	if (!window)
	{
		clean();
		SDL_Log("SDL failed to create window: %s", SDL_GetError());
		return 1;
	}

	load_media();
	return 0;
}
void emerge::clean()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}
void emerge::load_media()
{

}
int emerge::game_loop()
{
	while (1)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				clean();
				return 0;
			}
		}
	}
}
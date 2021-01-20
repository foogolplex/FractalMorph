#include "emerge.h"

int main(int ac, char* av[])
{
	emerge* instance;

	instance = new emerge();
	if (instance->init_sdl() != 0)
		return 0;
	instance->game_loop();
	return 0;
}
#include "morph.h"

int main(int ac, char* av[])
{
	Morph* instance;

	instance = new Morph();
	if (instance->init_sdl() != 0)
		return 0;
	instance->main_loop();
	return 0;
}

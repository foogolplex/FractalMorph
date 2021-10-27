#include <SDL2/SDL.h>
#include <string>
#include <cstdlib>
#include <time.h>
#include <math.h>
#pragma once

class Morph 
{
public:
	Morph();
	~Morph();

	int init_sdl();
	int main_loop();
	void clean();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Color colors[1000];	
	int window_width, window_height;
	float julia_radius;
	float constant;
	
    void draw_mandel();
	void draw_julia();
	void gen_rand_colors();
};


#include <SDL2/SDL.h>
#include <string>
#include <cstdlib>
#include <time.h>
#include <math.h>
#include <thread>
#include <vector>
#include <mutex>
#pragma once

class Morph 
{
public:
	Morph();
	~Morph();

	int init_sdl();
	int main_loop();
	void clean();
	void renderSlice(int start_y, int end_y, SDL_Renderer* renderer);
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	int window_width, window_height;
	float julia_radius;
	float constant;
	std::mutex rendererMutex;
	
    void draw_mandel();
	void draw_julia();
};


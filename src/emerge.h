#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cstdlib>
#include <time.h>
#include <complex>
#include <math.h>
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
	SDL_Surface* window_surface;
	SDL_Surface* background_image;
	SDL_Surface* player_sprite;
	SDL_Renderer* renderer;
	SDL_Color colors[1000];
	int player_x, player_y;
	int window_width, window_height;
	float julia_radius;
	float constant;
	void load_media();
	void draw_player();
	SDL_Surface* load_surface(std::string file_path);
	void draw();
	void draw_fractal(int type);
	void draw_julia();
	void gen_rand_colors();
};


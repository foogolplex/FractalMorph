#include "emerge.h"


enum FRACTAL_TYPE {
	MANDLEBROT,
	SIERPINSKI
};
emerge::emerge()
{
	window = NULL;
	background_image = NULL;
	window_surface = NULL;
	player_sprite = NULL;
	window_width = 1920;
	window_height = 1080;
	player_x = 0;
	player_y = 0;
	julia_radius = 4;
	constant = -2;
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
		"morph",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_width,
		window_height,
		SDL_WINDOW_RESIZABLE
		);
	if (!window)
	{
		clean();
		SDL_Log("SDL failed to create window: %s", SDL_GetError());
		return 1;
	}
	window_surface = SDL_GetWindowSurface(window);
	renderer = SDL_CreateRenderer(window, -1, 0);
	load_media();
	return 0;
}
void emerge::clean()
{
	SDL_FreeSurface(window_surface);
	SDL_FreeSurface(background_image);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
SDL_Surface* emerge::load_surface(std::string file_path)
{
	SDL_Surface* optimized_surface;
	SDL_Surface* loaded_surface;

	optimized_surface = NULL;
	/*loaded_surface = IMG_Load(file_path.c_str());
	if (loaded_surface == NULL)
	{
		SDL_Log("SDL was unable to load an image: %s", SDL_GetError());
	}
	else
	{
		optimized_surface = SDL_ConvertSurface(loaded_surface, window_surface->format, 0);
		if (optimized_surface == NULL)
			SDL_Log("SDL was unable to convert a surface: %s", SDL_GetError());
		SDL_FreeSurface(loaded_surface);
	}
	*/
	return optimized_surface;
}
void emerge::load_media()
{
	std::string bg_image_path = "lain.png";
	std::string player_sprite_path = "player.png";

	background_image = load_surface(bg_image_path);
	player_sprite = load_surface(player_sprite_path);
}
void emerge::draw_player()
{
	SDL_Rect player_dest;

	player_dest.x = player_x;
	player_dest.y = player_y;
	player_dest.w = 64;
	player_dest.h = 64;
	SDL_BlitScaled(player_sprite, NULL, window_surface, &player_dest);
}
void emerge::draw_julia()
{
	float scaled_x, scaled_y;
	int iteration, max_iteration;
	float xtemp;
	
	iteration = 0;
	max_iteration = 100;
	julia_radius = 2;
	for (int py = 0; py < window_height; py += 1)
	{
		for (int px = 0; px < window_width; px += 1)
		{
			scaled_x = ((julia_radius * 2.0f) / (float)window_width) * (float)px - julia_radius;
			scaled_y = ((julia_radius * 2.0f) / (float)window_height) * (float)py - julia_radius;
			
			iteration = 0;
			while (iteration < max_iteration && scaled_x * scaled_x + scaled_y * scaled_y <= julia_radius * julia_radius)
			{
				xtemp = scaled_x * scaled_x - scaled_y * scaled_y;
				scaled_y = 2.0f * scaled_x * scaled_y + constant;
				scaled_x = xtemp + constant;

				iteration += 1;
			}
			if (iteration == max_iteration)
			{
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderDrawPoint(renderer, px, py);
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, colors[iteration].r, colors[iteration].g, colors[iteration].b, 255);
				SDL_RenderDrawPoint(renderer, px, py);
			}
		}
	}
	
	/*
	std::complex<float> scaled_x, scaled_y, xtemp;
	std::complex<float> cconstant;
	int iteration = 0, max_iteration = 0;
	julia_radius = 2;
	std::complex<float> julia_cradius(julia_radius, 0.0f);
	for (int py = 0; py < window_height; py += 1)
	{
		for (int px = 0; px < window_width; px += 1)
		{
			scaled_x = std::complex<float>((julia_cradius.real() * 2.0) / (float)window_width * (float)px - julia_cradius.real(), 0.0f);
			scaled_y = std::complex<float>((julia_cradius.real() * 2.0) / (float)window_height * (float)py - julia_cradius.real(), 0.0f);
			cconstant = std::complex<float>(0.285, 0.0f);
			iteration = 0;
			while (scaled_x.real() * scaled_x.real() + scaled_y.real() * scaled_y.real() <= julia_cradius.real() * julia_cradius.real() && iteration < max_iteration)
			{
				xtemp = scaled_x * scaled_x - scaled_y * scaled_y;
				scaled_y = (float)2.0 * scaled_x * scaled_y + cconstant;
				scaled_x = xtemp + cconstant;

				iteration += 1;
			}
			if (iteration == max_iteration)
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			else
				SDL_SetRenderDrawColor(renderer, colors[iteration].r, colors[iteration].g, colors[iteration].b, 255);
			SDL_RenderDrawPoint(renderer, px, py);
		}
	}
	*/
	SDL_RenderPresent(renderer);
}
void emerge::draw_fractal(int type)
{
	int iteration, max_iteration;
	float x, y, x0, y0, x2, y2;
	float xtemp;

	if (type == MANDLEBROT)
	{
		for (int py = 0; py <= window_height; py += 1)
		{
			for (int px = 0; px <= window_width; px += 1)
			{
				x0 = (3.5 / window_width) * (float)px;
				y0 = (2.0 / window_height) * (float)py;
				x0 -= 2.5;
				y0 -= 1;
				x = 0.0;
				y = 0.0;
				x2 = 0;
				y2 = 0;
				iteration = 0;
				max_iteration = 1000;
				while (x2 + y2 <= 4 && iteration < max_iteration)
				{
					y = 2 * x * y + y0;
					x = x2 - y2 + x0;
					x2 = x * x;
					y2 = y * y;
					iteration += 1;
				}
				SDL_SetRenderDrawColor(renderer, colors[iteration].r, colors[iteration].g, colors[iteration].b, colors[iteration].a);
				SDL_RenderDrawPoint(renderer, px, py);
			}
		}
	}
	else if (type == SIERPINSKI)
	{

	}
	SDL_RenderPresent(renderer);
}
void emerge::gen_rand_colors()
{
	for (int i = 0; i < 1000; i += 1)
	{
		colors[i].r = rand() % 255;
		colors[i].g = rand() % 255;
		colors[i].b = rand() % 255;
		colors[i].a = 255;
	}
}
int emerge::game_loop()
{
	srand(time(NULL));
	gen_rand_colors();
	//draw_fractal(MANDLEBROT);
	draw_julia();
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
			else if (e.type == SDL_KEYDOWN)
			{
				int w, h;
				SDL_GetWindowSize(window, &w, &h);
				switch (e.key.keysym.sym)
				{
				case SDLK_UP:
					if(player_y - 10 >= 0)
						player_y -= 10;
					break;
				case SDLK_DOWN:
					if(player_y + 10 <= h - 60)
						player_y += 10;
					break;
				case SDLK_RIGHT:
					if (player_x + 10 <= w - 60)
						player_x += 10;
					constant += 0.01;
					break;
				case SDLK_LEFT:
					if (player_x - 10 >= 0)
						player_x -= 10;
					break;
				case SDLK_SPACE:
					srand(time(NULL));
					gen_rand_colors();
					draw_fractal(MANDLEBROT);
				}
			}
		}
		constant += 0.01;
		if (constant > 2)
		{
			srand(time(NULL));
			gen_rand_colors();
			constant = -1;
		}
		draw_julia();
		SDL_GetWindowSize(window, &window_width, &window_height);	
	}
}
void emerge::draw()
{
	SDL_Rect stretch_rect;

	stretch_rect.x = 0;
	stretch_rect.y = 0;
	stretch_rect.w = window_width;
	stretch_rect.h = window_height;
	window_surface = SDL_GetWindowSurface(window);
	SDL_BlitScaled(background_image, NULL, window_surface, &stretch_rect);
}

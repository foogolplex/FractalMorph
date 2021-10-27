#include "morph.h"

Morph::Morph()
{
	window = NULL;
	window_width = 1920;
	window_height = 1080;
	julia_radius = 4;
	constant = -2;
}
Morph::~Morph()
{}
int Morph::init_sdl()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		SDL_Log("SDL failed to initialize: %s", SDL_GetError());
		return 1;
	}
	
	window = SDL_CreateWindow(
		"Morph",
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
	renderer = SDL_CreateRenderer(window, -1, 0);
	return 0;
}
void Morph::clean()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}
void Morph::draw_julia()
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
			scaled_x = ((julia_radius * 2.0f) / (float)window_width) * 
                (float)px - julia_radius;
			scaled_y = ((julia_radius * 2.0f) / (float)window_height) * 
                (float)py - julia_radius;
			
			iteration = 0;
			while (iteration < max_iteration && scaled_x * scaled_x + scaled_y
                    * scaled_y <= julia_radius * julia_radius)
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
				SDL_SetRenderDrawColor(renderer, colors[iteration].r, 
                        colors[iteration].g, colors[iteration].b, 255);
				SDL_RenderDrawPoint(renderer, px, py);
			}
		}
	}	
	SDL_RenderPresent(renderer);
}
void Morph::draw_mandel()
{
	int iteration, max_iteration;
	float x, y, x0, y0, x2, y2;
	float xtemp;

	for (int py = 0; py <= window_height; py += 1)
	{
		for (int px = 0; px <= window_width; px += 1)
		{
			x0 = (3.5 / window_width) * (float)px;
			y0 = (2.0 / window_height) * (float)py;
			x0 -= 2.5;
			y0 -= 1;
			x, y = 0.0;
			x2, y2 = 0;
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
			SDL_SetRenderDrawColor(renderer, colors[iteration].r, 
                    colors[iteration].g, colors[iteration].b, colors[iteration].a);
			SDL_RenderDrawPoint(renderer, px, py);
		}
	}	
	SDL_RenderPresent(renderer);
}
void Morph::gen_rand_colors()
{
    // generate palette of colors whose value changes with the index
	for (int i = 0; i < 1000; i += 1)
	{
		colors[i].r = rand() % 255;
        colors[i].g = 0; 
		colors[i].b = 0; 
		colors[i].a = 255;
	}
}
int Morph::main_loop()
{
	srand(time(NULL));
	gen_rand_colors();	
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
				switch (e.key.keysym.sym)
				{	
				case SDLK_SPACE:
					srand(time(NULL));
					gen_rand_colors();
					draw_julia();
				}
			}
		}
		constant += 0.005;
		if (constant > 2)
		{
			srand(time(NULL));
			gen_rand_colors();
			constant = -2;
		}
		draw_julia();
		SDL_GetWindowSize(window, &window_width, &window_height);	
	}
}

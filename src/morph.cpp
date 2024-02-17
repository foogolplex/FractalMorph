#include "morph.h"

Morph::Morph()
{
	window = NULL;
	window_width = 600;
	window_height = 400;
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
		window_height, 0
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
void Morph::renderSlice(int start_y, int end_y, SDL_Renderer* renderer){
	std::lock_guard<std::mutex> lock(rendererMutex);
	for (int py = start_y; py < end_y; py += 1)
	{
		for (int px = 0; px < window_width; px += 1)
		{
			float scaled_x = ((julia_radius * 2.0f) / (float)window_width) * 
				(float)px - julia_radius;
			float scaled_y = ((julia_radius * 2.0f) / (float)window_height) * 
				(float)py - julia_radius;
			
			int iteration = 0;
			int max_iteration = 100;
			while (iteration < max_iteration && scaled_x * scaled_x + scaled_y
					* scaled_y <= julia_radius * julia_radius)
			{
				float xtemp = scaled_x * scaled_x - scaled_y * scaled_y;
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
				SDL_SetRenderDrawColor(renderer, 0, 
						0, iteration * 4, 255);
				SDL_RenderDrawPoint(renderer, px, py);
			}
		}
	}
	
}
void Morph::draw_julia()
{
	float scaled_x, scaled_y;
	const int num_threads = std::thread::hardware_concurrency();
	std::vector<std::thread> threads;
	julia_radius = 2;

	int slice_height = window_height / num_threads;
    int start_y = 0;

	// Attempt to distribute work across threads to render proportionate slices
	for (int i = 0; i < num_threads; ++i) {
        int end_y = (i == num_threads - 1) ? window_height : start_y + slice_height;
        threads.emplace_back([=]() { renderSlice(start_y, end_y, renderer); });
        start_y = end_y;
    }

    for (auto& thread : threads) {
        thread.join();
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
			SDL_SetRenderDrawColor(renderer, iteration, 
                    iteration, iteration, 255);
			SDL_RenderDrawPoint(renderer, px, py);
		}
	}	
	SDL_RenderPresent(renderer);
}
int Morph::main_loop()
{
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
					draw_julia();
				}
			}
		}
		constant += 0.005;
		if (constant > 2)
		{
			constant = -2;
		}
		draw_julia();
		SDL_GetWindowSize(window, &window_width, &window_height);	
	}
}

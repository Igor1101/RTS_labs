/*
 * app.cpp
 *
 *  Created on: Feb 19, 2020
 *      Author: igor
 */
#include "app.hpp"
#include "defs.h"



void App::init(void)
{
	init_sdl2();
}

void App::init(int width, int height, char*name)
{
	App::width = width;
	App::height = height;
	App::name = name;
	init_sdl2();
}

void App::init_sdl2(void)
{
	int rendererFlags, windowFlags;
	rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN;
	if (SDL_Init(SDL_INIT_VIDEO) < 0){
		pr_err("Couldn't initialize SDL: %s", SDL_GetError());
		exit(1);
	}
	using namespace std;
	win = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, windowFlags);
	if (!win){
		pr_err("Failed to open %d x %d window: %s", width, height, SDL_GetError());
		exit(1);
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	ren = SDL_CreateRenderer(win, -1, rendererFlags);
	if (!ren){
		pr_err("Failed to create renderer: %s", SDL_GetError());
		exit(1);
	}
}

void App::out(int x, int y)
{
	//int pperrealp = App::actual_height / App::height;
	const int pperrealp = 3;
	 SDL_Rect rect = {
		.x = x ,
		.y = y ,
		.w = pperrealp,
		.h = pperrealp
	};
	SDL_SetRenderDrawColor(ren, 96, 128, 255, 255);
	SDL_RenderDrawRect(ren, &rect);
}

void App::clear_win()
{
	SDL_SetRenderDrawColor(ren, 0,0,0,0);
	SDL_RenderClear(ren);
}

void App::draw_middleline()
{
	SDL_SetRenderDrawColor(ren, 10, 250, 240, 250);
	SDL_RenderDrawLine(ren, middle_x(), middle_y(), end_x(), middle_y());
}

void App::refresh_win()
{
	SDL_UpdateWindowSurface(win);
	SDL_RenderPresent(ren);
}

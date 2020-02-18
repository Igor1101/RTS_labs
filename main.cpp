#include <cstdio>
#include <cmath>
#include <SDL2/SDL.h>
#include "defs.h"
#include "statistics.hpp"
#include "RNG.h"
#define VAR_DISCR 256
#define VAR_W 1500
#define VAR_HARM 10
#define TARGET_NAME "LAB1"

class App{
private:
	int actual_width;
	int actual_height;
	void init_sdl2(void);
public:
	int width;
	int height;
	SDL_Renderer* ren;
	SDL_Window* win;
	void init(void);
	void init(int width, int height, int pixelperrealpixel);
	void init(int width, int height);
	void out(int x, int y);
	void exit_prog(int stat);
};


void App::init(void)
{
	init_sdl2();
}

void App::init(int width, int height, int pixelperrealpixel)
{
	App::width = width;
	App::height = height;
	App::actual_width = pixelperrealpixel * width;
	App::actual_height = pixelperrealpixel * height;
	init_sdl2();
}

void App::init(int width, int height)
{
	App::width = width;
	App::height = height;
	App::actual_width = width;
	App::actual_height = height;
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
	win = SDL_CreateWindow(TARGET_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, actual_width, actual_height, windowFlags);
	if (!win){
		pr_err("Failed to open %d x %d window: %s", actual_width, actual_height, SDL_GetError());
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

double harmonic(int t, int w, double ampl, double phi){
	return ampl*sin((double)w*t+phi);
}
int main(int argc, char **argv) {
	RNG_init();
	App app;
	app.init(1024, 768, 1);
	SDL_SetRenderDrawColor(app.ren, 0,0,0,0);
	SDL_RenderClear(app.ren);
	SDL_SetRenderDrawColor(app.ren, 0,0,0,0);
	double ampl = RNG.get_float(0, 1);
	double phi = RNG.get_float(0, 1);
	printf("ampl=%lf, phi=%lf\n", ampl, phi);
	for(int t=0; t<VAR_DISCR; t++) {
		double x, x_prev;
		x_prev = x;
		x=0;
		for(int harm=0; harm<VAR_HARM; harm++) {
			x += harmonic(t, harm*VAR_DISCR/VAR_HARM, ampl, phi);
		//SDL_RenderDrawPoint(app.ren, t*3, -x*10+app.height/2);
		}
		printf("x=%lf\n", x);
		//app.out(t,x+app.height/2);
		//SDL_SetRenderDrawColor(app.ren, 96, 128, 255, 255);
		//SDL_RenderDrawPoint(app.ren, t*3, -x*20+app.height/2);
		app.out(t*3, -x*20+app.height/2);
		SDL_SetRenderDrawColor(app.ren, 96, 128, 0, 255);
		SDL_RenderDrawLine(app.ren, t*3, -x*20+app.height/2, (t-1)*3, -x_prev*20+app.height/2);
	}
	SDL_UpdateWindowSurface(app.win);
	SDL_RenderPresent(app.ren);
	while (1){
		SDL_Event event;
		SDL_PollEvent(&event);
		if(event.type == SDL_QUIT || event.key.keysym.sym == 'q') {
			SDL_DestroyRenderer(app.ren);
			SDL_DestroyWindow(app.win);
			SDL_Quit();
			exit(0);
		}
	}
	return 0;
}

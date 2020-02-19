#include <cstdio>
#include <cmath>
#include <SDL2/SDL.h>
#include <algorithm>
#include <iostream>
#include "defs.h"
#include "statistics.hpp"
#include "RNG.h"
#include "app.hpp"
#define VAR_DISCR 256
#define VAR_W 1500
#define VAR_HARM 10

double x_arr[VAR_DISCR];
int t_arr[VAR_DISCR];

double harmonic(int t, int w, double ampl, double phi){
	return ampl*sin(((double)w)*t+phi);
}
int main(int argc, char **argv) {
	RNG_init();
	App app;
	app.init(1024, 768, 1);
	SDL_SetRenderDrawColor(app.ren, 0,0,0,0);
	SDL_RenderClear(app.ren);
	SDL_SetRenderDrawColor(app.ren, 0,0,0,0);
	double ampl = 0.5;//RNG.get_float(0, 1);
	double phi = 0.5;//RNG.get_float(0, 1);
	printf("ampl=%lf, phi=%lf\n", ampl, phi);
	// here draw line t=0
	SDL_SetRenderDrawColor(app.ren, 10, 250, 240, 250);
	SDL_RenderDrawLine(app.ren, app.middle_x(), app.middle_y(), app.end_x(), app.middle_y());
		double x, x_prev;
	for(int t=0; t<VAR_DISCR; t++) {
		x_prev = x;
		x=0;
		for(int harm=0; harm<VAR_HARM; harm++) {
			x += harmonic(t, harm*VAR_DISCR/VAR_HARM, ampl, phi);
		//SDL_RenderDrawPoint(app.ren, t*3, -x*30+app.height/2);
		}
		x_arr[t] = x;
		t_arr[t] = t;
		printf("x=%lf\n", x);
	}
	// draw x(t)
	std::pair<double*, double*> minmaxx = std::minmax_element(std::begin(x_arr), std::end(x_arr));
	std::pair<int*, int*> minmaxt = std::minmax_element(std::begin(t_arr), std::end(t_arr));
	// conv
	double x_offs = (abs(*(minmaxx.first))>abs(*(minmaxx.second)))?abs(*(minmaxx.first)):abs(*(minmaxx.second));
	int t_offs = (abs(*(minmaxt.first))>abs(*(minmaxt.second)))?abs(*(minmaxt.first)):abs(*(minmaxt.second));
	double x_coef = (app.end_y() - app.middle_y()) / x_offs;
	double t_coef = (app.end_x() - app.middle_x()) / t_offs;
	for(int i=0; i<VAR_DISCR; i++) {
		app.out(t_arr[i]*t_coef, app.real_y(x_arr[i]*x_coef));
		SDL_SetRenderDrawColor(app.ren, 10, 150, 0, 0);
		if(i+1<VAR_DISCR)
			SDL_RenderDrawLine(app.ren, t_arr[i]*t_coef, app.real_y(x_arr[i]*x_coef), (t_arr[i+1]*t_coef), app.real_y(x_arr[i+1]*x_coef));
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

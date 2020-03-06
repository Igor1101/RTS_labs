#include <cstdio>
#include <cmath>
#include <SDL2/SDL.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <ctime>
#include "defs.h"
#include "statistics.hpp"
#include "RNG.h"
#include "app.hpp"
using namespace std;
#define VAR_DISCR  1024
#define VAR_W 1500
#define VAR_HARM 10

double x_arr[VAR_DISCR];
double y_arr[VAR_DISCR];
double Rxx_arr[VAR_DISCR];
double Rxy_arr[VAR_DISCR];
int t_arr[VAR_DISCR];
double harmonic(int t, int w, double ampl, double phi){
	return ampl*sin(((double)w)*t+phi);
}
int main(int argc, char **argv) {
	RNG_init();
	App appx, appy, appcorxx, appcorxy;
	// drawing windows
	appx.init(1024, 768, (char*)"X");
	appy.init(1024, 768, (char*)"Y");
	appcorxx.init(1024, 768, (char*)"Rxx");
	appcorxy.init(1024, 768, (char*)"Rxy");
	appx.clear_win();
	double amplx = RNG.get_float(0, 1);
	double phix = RNG.get_float(0, 1);
	printf("amplx=%lf, phix=%lf\n", amplx, phix);
	// here draw line t=0
	appx.draw_middleline();
	appy.draw_middleline();
	appcorxx.draw_middleline();
	appcorxy.draw_middleline();
	// find x array
	double x;
	for(int t=0; t<VAR_DISCR; t++) {
		x=0;
		for(int harm=0; harm<VAR_HARM; harm++) {
			x += harmonic(t, harm*VAR_DISCR/VAR_HARM, amplx, phix);
		}
		x_arr[t] = x;
		t_arr[t] = t;
		printf("x[%i]=%f\n\r", t, x_arr[t]);
	}

	double amply = RNG.get_float(0, 1);
	double phiy = RNG.get_float(0, 1);
	printf("amply=%lf, phiy=%lf\n", amply, phiy);
	double y;
	for(int t=0; t<VAR_DISCR; t++) {
		y=0;
		for(int harm=0; harm<VAR_HARM; harm++) {
			y += harmonic(t, harm*VAR_DISCR/VAR_HARM, amply, phiy);
		}
		y_arr[t] = y;
		t_arr[t] = t;
		printf("y=%lf\n", y);
	}

	// draw x(t)
	std::pair<double*, double*> minmaxx = std::minmax_element(std::begin(x_arr), std::end(x_arr));
	std::pair<int*, int*> minmaxt = std::minmax_element(std::begin(t_arr), std::end(t_arr));
	// conv
	double x_offs = (abs(*(minmaxx.first))>abs(*(minmaxx.second)))?abs(*(minmaxx.first)):abs(*(minmaxx.second));
	int t_offs = (abs(*(minmaxt.first))>abs(*(minmaxt.second)))?abs(*(minmaxt.first)):abs(*(minmaxt.second));
	double x_coef = (appx.end_y() - appx.middle_y()) / x_offs;
	double t_coef = (appx.end_x() - appx.middle_x()) / t_offs;
	for(int i=0; i<VAR_DISCR; i++) {
		appx.out(t_arr[i]*t_coef, appx.real_y(x_arr[i]*x_coef));
		SDL_SetRenderDrawColor(appx.ren, 10, 150, 0, 0);
		if(i+1<VAR_DISCR)
			SDL_RenderDrawLine(appx.ren, t_arr[i]*t_coef, appx.real_y(x_arr[i]*x_coef), (t_arr[i+1]*t_coef), appx.real_y(x_arr[i+1]*x_coef));
	}
	appx.refresh_win();
	// draw y(t)
	std::pair<double*, double*> minmaxy = std::minmax_element(std::begin(y_arr), std::end(y_arr));
	//std::pair<int*, int*> minmaxt = std::minmax_element(std::begin(t_arr), std::end(t_arr));
	// conv
	double y_offs = (abs(*(minmaxy.first))>abs(*(minmaxy.second)))?abs(*(minmaxy.first)):abs(*(minmaxy.second));
	//int t_offs = (abs(*(minmaxt.first))>abs(*(minmaxt.second)))?abs(*(minmaxt.first)):abs(*(minmaxt.second));
	double y_coef = (appy.end_y() - appy.middle_y()) / y_offs;
	for(int i=0; i<VAR_DISCR; i++) {
		appy.out(t_arr[i]*t_coef, appy.real_y(y_arr[i]*y_coef));
		SDL_SetRenderDrawColor(appy.ren, 10, 150, 0, 0);
		if(i+1<VAR_DISCR)
			SDL_RenderDrawLine(appy.ren, t_arr[i]*t_coef, appy.real_y(y_arr[i]*y_coef), (t_arr[i+1]*t_coef), appy.real_y(y_arr[i+1]*y_coef));
	}
	appy.refresh_win();


	clock_t start_my, end_my, start_dy, end_dy;
	start_my = clock();
	double My = Expected(y_arr, VAR_DISCR);
	end_my = clock();
	double my_timeused = ((double) (end_my - start_my)) / CLOCKS_PER_SEC;
	start_dy = clock();
	double Dy = Dispersion(y_arr, VAR_DISCR);
	end_dy = clock();
	double dy_timeused = ((double) (end_dy - start_dy)) / CLOCKS_PER_SEC;
	printf("dy time=%f\n\r", dy_timeused);
	printf("my time=%f\n\r", my_timeused);

	clock_t start_mx, end_mx, start_dx, end_dx;
	start_mx = clock();
	double Mx = Expected(x_arr, VAR_DISCR);
	end_mx = clock();
	double mx_timeused = ((double) (end_mx - start_mx)) / CLOCKS_PER_SEC;
	start_dx = clock();
	double Dx = Dispersion(x_arr, VAR_DISCR);
	end_dx = clock();
	double dx_timeused = ((double) (end_dx - start_dx)) / CLOCKS_PER_SEC;
	printf("dx time=%f\n\r", dx_timeused);
	printf("mx time=%f\n\r", mx_timeused);

	clock_t start_Rxx, end_Rxx, start_Rxy, end_Rxy;
	// get autocorrelation
	start_Rxx = clock();
	for(int tau=0; tau<VAR_DISCR; tau++) {
		Rxx_arr[tau] = 0;
		for(int i=tau; i<VAR_DISCR-tau; i++) {
			 double Rxx = (x_arr[i] - Mx) * (x_arr[i+tau] - Mx);
			 Rxx_arr[tau] += Rxx;
		}
		Rxx_arr[tau] /= (VAR_DISCR/2-1);
		//printf("Rxx[%i]=%f\n\r", tau, Rxx_arr[tau]);
	}
	end_Rxx = clock();
	// draw Rxx(t)
	std::pair<double*, double*> minmaxRxx = std::minmax_element(std::begin(Rxx_arr), std::end(Rxx_arr));
	//std::pair<int*, int*> minmaxt = std::minmax_element(std::begin(t_arr), std::end(t_arr));
	// conv
	double Rxx_offs = (abs(*(minmaxRxx.first))>abs(*(minmaxRxx.second)))?abs(*(minmaxRxx.first)):abs(*(minmaxRxx.second));
	double Rxx_coef = (appcorxx.end_y() - appcorxx.middle_y()) / Rxx_offs;
	for(int i=0; i<VAR_DISCR; i++) {
		appcorxx.out(t_arr[i]*t_coef, appcorxx.real_y(Rxx_arr[i]*Rxx_coef));
		SDL_SetRenderDrawColor(appcorxx.ren, 10, 150, 0, 0);
		if(i+1<VAR_DISCR)
			SDL_RenderDrawLine(appcorxx.ren, t_arr[i]*t_coef, appcorxx.real_y(Rxx_arr[i]*Rxx_coef), (t_arr[i+1]*t_coef), appcorxx.real_y(Rxx_arr[i+1]*Rxx_coef));
	}
	appcorxx.refresh_win();


	// get correlation
	start_Rxy = clock();
	for(int tau=0; tau<VAR_DISCR; tau++) {
		Rxy_arr[tau] = 0;
		for(int i=tau; i<VAR_DISCR-tau; i++) {
			 double Rxy = (x_arr[i] - Mx) * (y_arr[i+tau] - Mx);
			 Rxy_arr[tau] += Rxy;
		}
		Rxy_arr[tau] /= (VAR_DISCR/2-1);
		//printf("Rxy[%i]=%f\n\r", tau, Rxy_arr[tau]);
	}
	end_Rxy = clock();
	// draw Rxx(t)
	std::pair<double*, double*> minmaxRxy = std::minmax_element(std::begin(Rxy_arr), std::end(Rxy_arr));
	//std::pair<int*, int*> minmaxt = std::minmax_element(std::begin(t_arr), std::end(t_arr));
	// conv
	double Rxy_offs = (abs(*(minmaxRxy.first))>abs(*(minmaxRxy.second)))?abs(*(minmaxRxy.first)):abs(*(minmaxRxy.second));
	double Rxy_coef = (appcorxy.end_y() - appcorxy.middle_y()) / Rxy_offs;
	for(int i=0; i<VAR_DISCR; i++) {
		appcorxy.out(t_arr[i]*t_coef, appcorxy.real_y(Rxy_arr[i]*Rxy_coef));
		SDL_SetRenderDrawColor(appcorxy.ren, 10, 150, 0, 0);
		if(i+1<VAR_DISCR)
			SDL_RenderDrawLine(appcorxy.ren, t_arr[i]*t_coef, appcorxy.real_y(Rxy_arr[i]*Rxy_coef), (t_arr[i+1]*t_coef), appcorxy.real_y(Rxy_arr[i+1]*Rxy_coef));
	}
	appcorxy.refresh_win();


	double Rxx_timeused = ((double) (end_Rxx - start_Rxx)) / CLOCKS_PER_SEC;
	double Rxy_timeused = ((double) (end_Rxy - start_Rxy)) / CLOCKS_PER_SEC;
	// get xy cor
	// write results to file
	ofstream file;
	file.open("lab_res.txt");
	file << "dx time=" << dx_timeused << endl;
	file << "mx time=" << mx_timeused << endl;
	file << "Mx=" << Mx << endl;
	file << "Dx=" << Dx << endl;
	file << "dy time=" << dy_timeused << endl;
	file << "my time=" << my_timeused << endl;
	file << "My=" << My << endl;
	file << "Dy=" << Dy << endl;
	file << "Rxx time=" << Rxx_timeused << endl;
	file << "Rxy time=" << Rxy_timeused << endl;
	file.close();
	// get it into google drive
	extern int gdrive_out();
	//gdrive_out();
	while (1){
		SDL_Event event;
		SDL_PollEvent(&event);
		if(event.type == SDL_QUIT || event.key.keysym.sym == 'q') {
			SDL_DestroyRenderer(appx.ren);
			SDL_DestroyWindow(appx.win);
			SDL_Quit();
			exit(0);
		}
	}
	return 0;
}

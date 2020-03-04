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
#define VAR_DISCR 256
#define VAR_W 1500
#define VAR_HARM 10

double x_arr[VAR_DISCR];
double y_arr[VAR_DISCR];
double Rp_arr[VAR_DISCR];
double Rxy_arr[VAR_DISCR];
int t_arr[VAR_DISCR];

double harmonic(int t, int w, double ampl, double phi){
	return ampl*sin(((double)w)*t+phi);
}
int main(int argc, char **argv) {
	RNG_init();
	App appx, appy, appcorxy;
	// drawing windows
	appx.init(1024, 768, (char*)"X");
	appy.init(1024, 768, (char*)"Y");
	appcorxy.init(1024, 768, (char*)"Rxx");
	appx.clear_win();
	double ampl = RNG.get_float(0, 1);
	double phi = RNG.get_float(0, 1);
	printf("ampl=%lf, phi=%lf\n", ampl, phi);
	// here draw line t=0
	appx.draw_middleline();
	appy.draw_middleline();
	appcorxy.draw_middleline();
	// find x array
	double x;
	for(int t=0; t<VAR_DISCR; t++) {
		x=0;
		for(int harm=0; harm<VAR_HARM; harm++) {
			x += harmonic(t, harm*VAR_DISCR/VAR_HARM, ampl, phi);
		}
		x_arr[t] = x;
		t_arr[t] = t;
		printf("x=%lf\n", x);
	}

	ampl = RNG.get_float(0, 1);
	phi = RNG.get_float(0, 1);
	double y;
	for(int t=0; t<VAR_DISCR; t++) {
		y=0;
		for(int harm=0; harm<VAR_HARM; harm++) {
			y += harmonic(t, harm*VAR_DISCR/VAR_HARM, ampl, phi);
		//SDL_RenderDrawPoint(appx.ren, t*3, -x*30+appx.height/2);
		}
		y_arr[t] = y;
		t_arr[t] = t;
		printf("x=%lf\n", y);
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
	SDL_UpdateWindowSurface(appx.win);
	SDL_RenderPresent(appx.ren);

	// draw y(t)
	std::pair<double*, double*> minmaxy = std::minmax_element(std::begin(y_arr), std::end(y_arr));
	//std::pair<int*, int*> minmaxt = std::minmax_element(std::begin(t_arr), std::end(t_arr));
	// conv
	double y_offs = (abs(*(minmaxy.first))>abs(*(minmaxy.second)))?abs(*(minmaxy.first)):abs(*(minmaxy.second));
	//int t_offs = (abs(*(minmaxt.first))>abs(*(minmaxt.second)))?abs(*(minmaxt.first)):abs(*(minmaxt.second));
	double y_coef = (appy.end_y() - appy.middle_y()) / y_offs;
	//double t_coef = (appy.end_x() - appy.middle_x()) / t_offs;
	for(int i=0; i<VAR_DISCR; i++) {
		appy.out(t_arr[i]*t_coef, appy.real_y(y_arr[i]*x_coef));
		SDL_SetRenderDrawColor(appy.ren, 10, 150, 0, 0);
		if(i+1<VAR_DISCR)
			SDL_RenderDrawLine(appy.ren, t_arr[i]*t_coef, appy.real_y(y_arr[i]*x_coef), (t_arr[i+1]*t_coef), appx.real_y(y_arr[i+1]*y_coef));
	}
	SDL_UpdateWindowSurface(appy.win);
	SDL_RenderPresent(appy.ren);

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

	// get autocorrelation
	double Rxx=0;
	double Rxx_result=0;
	for(int i=0; i<VAR_DISCR/2; i++) {
		Rxx=0;
		for(int j=VAR_DISCR/2; j<VAR_DISCR; j++) {
			 double Rp = (x_arr[i] - Mx) * (x_arr[j] - Mx);
			 Rxx += Rp;
			 Rp_arr[i] = Rp;
		}
		Rxx_result += Rxx;
	}

	// draw Rxx(t)
	std::pair<double*, double*> minmaxRp = std::minmax_element(std::begin(Rp_arr), std::end(Rp_arr));
	//std::pair<int*, int*> minmaxt = std::minmax_element(std::begin(t_arr), std::end(t_arr));
	// conv
	double Rp_offs = (abs(*(minmaxy.first))>abs(*(minmaxy.second)))?abs(*(minmaxy.first)):abs(*(minmaxy.second));
	//int t_offs = (abs(*(minmaxt.first))>abs(*(minmaxt.second)))?abs(*(minmaxt.first)):abs(*(minmaxt.second));
	double Rp_coef = (appcorxy.end_y() - appcorxy.middle_y()) / y_offs;
	//double t_coef = (appy.end_x() - appy.middle_x()) / t_offs;
	for(int i=0; i<VAR_DISCR; i++) {
		appcorxy.out(t_arr[i]*t_coef, appcorxy.real_y(y_arr[i]*Rp_coef));
		SDL_SetRenderDrawColor(appcorxy.ren, 10, 150, 0, 0);
		if(i+1<VAR_DISCR)
			SDL_RenderDrawLine(appcorxy.ren, t_arr[i]*t_coef, appcorxy.real_y(Rp_arr[i]*Rp_coef), (t_arr[i+1]*t_coef), appcorxy.real_y(Rp_arr[i+1]*Rp_coef));
	}
	SDL_UpdateWindowSurface(appcorxy.win);
	SDL_RenderPresent(appcorxy.ren);


	// get xy cor
	double Rxy=0;

	for(int i=0; i<VAR_DISCR/2; i++) {
		for(int j=0; j<VAR_DISCR/2; j++) {
			 Rxy += (x_arr[i] - Mx) * (y_arr[j] - My);
		}
	}
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
	file << "Rxx=" << Rxx_result << endl;
	//file << "Rxy=" << Rxy_result << endl;
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

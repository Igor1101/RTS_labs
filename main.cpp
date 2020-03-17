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
#include <new>
using namespace std;
#define VAR_DISCR  256
#define VAR_W 1500
#define VAR_HARM 10
double x_arr[VAR_DISCR];
int t_arr[VAR_DISCR];
double harmonic(int t, int w, double ampl, double phi){
	return ampl*sin(((double)w)*t+phi);
}
double freal[VAR_DISCR];
double fimage[VAR_DISCR];
double DFT_Farr[VAR_DISCR];
typedef struct {
	double Re;
	double Im;
}comple;
typedef comple * complep;
comple** Ws;
void DFT_wcalc(int N) {
	Ws = new complep[N];
	for(int p=0; p<N; p++) {
		Ws[p] = new comple[N];
		for(int k=0; k<N; k++) {
			double Re = cos(2 * M_PI / N * p * k);
			double Im = sin(2 * M_PI / N * p * k);
			Ws[p][k].Im = Im;
			Ws[p][k].Re = Re;
		}
	}
}
void DFT_f(void)
{
	const int N = VAR_DISCR;
	for(int p=0; p<N; p++) {
		freal[p] = 0;
		fimage[p] = 0;
		for(int k=0; k<N; k++) {
			freal[p] += x_arr[k] * Ws[p][k].Re;
			fimage[p] += x_arr[k] * Ws[p][k].Im;
		}
		DFT_Farr[p] = sqrt(freal[p] * freal[p] + fimage[p]*fimage[p]);
	}
}
int main(int argc, char **argv) {
	RNG_init();
	DFT_wcalc(VAR_DISCR);
	App appx, app_dft;
	// drawing windows
	appx.init(1024, 768, (char*)"X");
	app_dft.init(1024,768, (char*)"DFT F");
	appx.clear_win();
	app_dft.clear_win();
	double amplx = RNG.get_float(0, 1);
	double phix = RNG.get_float(0, 1);
	printf("amplx=%lf, phix=%lf\n", amplx, phix);
	// here draw line t=0
	appx.draw_middleline();
	app_dft.draw_middleline();
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
	// Find DFT
	DFT_f();
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


	// draw x DFT spectrum(t)
	std::pair<double*, double*> minmax_dft = std::minmax_element(std::begin(DFT_Farr), std::end(DFT_Farr));
	// conv
	double dft_offs = (abs(*(minmax_dft.first))>abs(*(minmax_dft.second)))?abs(*(minmax_dft.first)):abs(*(minmax_dft.second));
	double dft_coef = (app_dft.end_y() - app_dft.middle_y()) / dft_offs;
	for(int i=0; i<VAR_DISCR; i++) {
		app_dft.out(t_arr[i]*t_coef, app_dft.real_y(DFT_Farr[i]*dft_coef));
		SDL_SetRenderDrawColor(app_dft.ren, 10, 150, 0, 0);
		if(i+1<VAR_DISCR)
			SDL_RenderDrawLine(app_dft.ren, t_arr[i]*t_coef, app_dft.real_y(DFT_Farr[i]*dft_coef), (t_arr[i+1]*t_coef), app_dft.real_y(DFT_Farr[i+1]*dft_coef));
	}
	app_dft.refresh_win();


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
	// write results to file
	ofstream file;
	file.open("lab_res.txt");
	file << "dx time=" << dx_timeused << endl;
	file << "mx time=" << mx_timeused << endl;
	file << "Mx=" << Mx << endl;
	file << "Dx=" << Dx << endl;
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

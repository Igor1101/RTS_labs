#include <cstdio>
#include <cmath>
#include <SDL2/SDL.h>
#include <algorithm>
#include <iostream>
#include <new>
#include <fstream>
#include <ctime>
#include "defs.h"
#include "statistics.hpp"
#include "RNG.h"
#include "app.hpp"
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
double FFT_Farr[VAR_DISCR];

typedef struct {
	double r;
	double i;
}w_type;

void w_mac( w_type* cc, w_type a, w_type w, w_type b )
{
	cc->r = a.r + w.r * b.r - w.i * b.i;
	cc->i = a.i + w.r * b.i + w.i * b.r;
}

void floating_to_w(w_type result[], double arr[], int n)
{
	for(int i=0; i<n; i++) {
		result[i].r = arr[i];
		result[i].i = 0;
	}
}

void w_to_floating(double result[], w_type arr[], int n)
{
	for(int i=0; i<n; i++) {
		result[i] = sqrt(arr[i].r * arr[i].r + arr[i].i * arr[i].i);
	}
}

void FFT_f(w_type result[], w_type arr[], int n)
{
	// stop recursion here
	if(n == 1)
		return;
	// additional arrays
	w_type* A = new w_type[n/2];
	w_type* B = new w_type[n/2];
	// fill them accordingly with shift
	for(int i=0; i<n/2; i++) {
		A[i] = arr[i*2];
		B[i] = arr[i*2+1];
	}
	//
	FFT_f(A, A, n/2);
	FFT_f(B, B, n/2);
	w_type w;
	for(int i=0; i<n; i++) {
		w.r = cos(2 * M_PI / n * i);
		w.i = sin(2 * M_PI / n * i);
		w_mac(&result[i], A[i% (n/2)], w, B[i % (n/2)]);
	}
	delete []A;
	delete []B;
}

int main(int argc, char **argv) {
	RNG_init();
	App appx, app_dft;
	// drawing windows
	appx.init(1024, 768, (char*)"X");
	app_dft.init(1024,768, (char*)"FFT F");
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
	// Find FFT
	w_type* x_arrw = new w_type[VAR_DISCR];
	floating_to_w(x_arrw, x_arr, VAR_DISCR);
	FFT_f(x_arrw, x_arrw, VAR_DISCR);
	w_to_floating(FFT_Farr, x_arrw, VAR_DISCR);
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


	// draw x FFT spectrum(t)
	std::pair<double*, double*> minmax_dft = std::minmax_element(std::begin(FFT_Farr), std::end(FFT_Farr));
	// conv
	double dft_offs = (abs(*(minmax_dft.first))>abs(*(minmax_dft.second)))?abs(*(minmax_dft.first)):abs(*(minmax_dft.second));
	double dft_coef = (app_dft.end_y() - app_dft.middle_y()) / dft_offs;
	for(int i=0; i<VAR_DISCR; i++) {
		app_dft.out(t_arr[i]*t_coef, app_dft.real_y(FFT_Farr[i]*dft_coef));
		SDL_SetRenderDrawColor(app_dft.ren, 10, 150, 0, 0);
		if(i+1<VAR_DISCR)
			SDL_RenderDrawLine(app_dft.ren, t_arr[i]*t_coef, app_dft.real_y(FFT_Farr[i]*dft_coef), (t_arr[i+1]*t_coef), app_dft.real_y(FFT_Farr[i+1]*dft_coef));
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

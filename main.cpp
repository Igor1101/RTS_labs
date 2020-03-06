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
double z_arr[VAR_DISCR];
double Rzz_arr[VAR_DISCR];
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
	clock_t clockxy, clockxyend;
	clockxy = clock();
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

	clockxyend = clock();
	clock_t clock_z, clock_zend;
	clock_z = clock();
	// find z array
	double z;
	for(int t=0; t<VAR_DISCR; t++) {
		z=0;
		for(int harm=0; harm<VAR_HARM; harm++) {
			z += harmonic(t, harm*VAR_DISCR/VAR_HARM, amplx, phix);
		}
		z_arr[t] = z;
		t_arr[t] = t;
		printf("x[%i]=%f\n\r", t, x_arr[t]);
	}

	double Mz = Expected(z_arr, VAR_DISCR);
	for(int tau=0; tau<VAR_DISCR; tau++) {
		Rzz_arr[tau] = 0;
		for(int i=tau; i<VAR_DISCR-tau; i++) {
			 double Rzz = (z_arr[i] - Mx) * (z_arr[i+tau] - Mx);
			 Rzz_arr[tau] += Rzz;
		}
		Rzz_arr[tau] /= (VAR_DISCR/2-1);
		//printf("Rxx[%i]=%f\n\r", tau, Rxx_arr[tau]);
	}
	clock_zend = clock();
	// draw Rxx(t)
	appcorxy.refresh_win();


	double clockxy_time = ((double) (clockxyend - clockxy)) / CLOCKS_PER_SEC;
	double clockz_time = ((double) (clock_zend- clock_z)) / CLOCKS_PER_SEC;
	// get xy cor
	// write results to file
	ofstream file;
	file.open("lab_res.txt");
	cout << "xy time=" << clockxy_time << endl;
	cout << "zz time=" << clockz_time<< endl;
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

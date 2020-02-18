/*
 * probability.h
 *
 *  Created on: Mar 8, 2019
 *      Author: igor
 */

#ifndef STATISTICS_HPP_
#define STATISTICS_HPP_

/*
 * generate expected value from values given in
 * provided array;
 * @param values : array
 * @param amount : amount of given data
 */
double Expected(double* values, int amount);
float Expected(float* values, int amount);
float Expected(float* values1, float* values2, int amount);
double Expected(double* values1, double* values2, int amount);
float Expected(float* values1, float* values2, float* values3, int amount);
double Expected(double* values1, double* values2, double*values3, int amount);
float Expected(float* values1, float* values2, float* values3, float* values4, int amount);
double Expected(double* values1, double* values2, double* values3, double* values4, int amount);
float Expected(float* v1, float* v2, float* v3, float* v4, float* v5, int amount);
double Expected(double* v1, double* v2, double* v3, double* v4, double* v5, int amount);
float Expected(float* v1, float* v2, float* v3, float* v4, float* v5, float*v6, int amount);
double Expected(double* v1, double* v2, double* v3, double* v4, double* v5, double*v6, int amount);

/*
 * generate dispersion from values given in
 * provided array;
 * @param values : array
 * @param amount : amount of given data
 */

double Dispersion(double* values,int amount);
float Dispersion(float* values,int amount);

/*
 * generate covariation from values given in
 * provided array;
 * @param x: x array
 * @param y: y array
 * @param amount : amount of given data
 */
double COV(double *x, double *y, int amount);
float COV(float *x, float *y, int amount);

double COR(double *x, double *y, int amount);
float COR(float *x, float*y, int amount);




#endif /* STATISTICS_HPP_ */

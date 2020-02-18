/*
 * probability.c
 *
 *  Created on: Mar 8, 2019
 *      Author: igor
 */

#include <cstdlib>
#include <cstdio>
#include <cmath>

#include "statistics.hpp"

double COR(double *x, double *y, int amount)
{
        double cov = COV(x,y,amount);
        double dev_x = sqrt(Dispersion(x, amount));
        double dev_y = sqrt(Dispersion(y, amount));
        return cov / ( dev_x * dev_y );
}

float COR(float *x, float *y, int amount)
{
        float cov = COV(x,y,amount);
        float dev_x = sqrt(Dispersion(x, amount));
        float dev_y = sqrt(Dispersion(y, amount));
        return cov / ( dev_x * dev_y );
}
/*
 * generate standard deviation from values given in
 * provided array;
 * @param values : array
 * @param amount : amount of given data
 */

double Dispersion(double* values,int amount)
{
        double Mv = Expected(values, amount);
        double *square_values = (double*)malloc(amount * sizeof(double));
        if(square_values == NULL)/* something`s dead ;( */ {
                puts("can`t allocate buffer");
                exit(-1);
        }
        for(int i=0; i < amount; i++) {
                square_values[i] = (values[i] - Mv) * (values[i] - Mv);
                //printf("val%f\n", square_values[i]);
        }
        double D = Expected(square_values, amount);
        /* now dev=dispersion, but deviation = sqrt(dispersion) */
        free(square_values);
        return D;
}

float Dispersion(float* values,int amount)
{
        float Mv = Expected(values, amount);
        float *square_values = (float *)malloc(amount * sizeof(float));
        if(square_values == NULL)/* something`s dead ;( */ {
                puts("can`t allocate buffer");
                exit(-1);
        }
        for(int i=0; i < amount; i++) {
                square_values[i] = (values[i] - Mv) * (values[i] - Mv);
                //printf("val%f\n", square_values[i]);
        }
        float D = Expected(square_values, amount);
        /* now dev=dispersion, but deviation = sqrt(dispersion) */
        free(square_values);
        return D;
}

/*
 * generate covariation from values given in
 * provided array;
 * @param x: x array
 * @param y: y array
 * @param amount : amount of given data
 */
double COV(double *x, double *y, int amount)
{
        double cov = 0;
        for(int i=0; i < amount; i++) {
                cov += x[i] * y[i];
        }
        cov = cov / (amount);
        cov -= Expected(x, amount) * Expected(y, amount);
        return cov;
}

float COV(float *x, float *y, int amount)
{
        float cov = 0;
        for(int i=0; i < amount; i++) {
                cov += x[i] * y[i];
        }
        cov = cov / (amount);
        cov -= Expected(x, amount) * Expected(y, amount);
        return cov;
}

/*
 * generate expected value from values given in
 * provided array;
 * @param values : array
 * @param amount : amount of given data
 */
double Expected(double* values, int amount)
{
        double expec_val = 0;
        for(int i=0; i < amount; i++) {
                expec_val += values[i] / amount;
        }
        return expec_val;
}

float Expected(float* values, int amount)
{
        float expec_val = 0;
        for(int i=0; i < amount; i++) {
                expec_val += values[i] / amount;
        }
        return expec_val;
}

float Expected(float* values1, float* values2, int amount)
{
        float expec_val = 0;
        for(int i=0; i < amount; i++) {
                expec_val += values1[i] * values2[i] / amount;
        }
        return expec_val;
}

double Expected(double* values1, double* values2, int amount)
{
        double expec_val = 0;
        for(int i=0; i < amount; i++) {
                expec_val += values1[i] * values2[i] / amount;
        }
        return expec_val;
}

float Expected(float* values1, float* values2, float* values3, int amount)
{
        float expec_val = 0;
        for(int i=0; i < amount; i++) {
                expec_val += values1[i] * values2[i] * values3[i] / amount;
        }
        return expec_val;
}

double Expected(double* values1, double* values2, double*values3, int amount)
{
        double expec_val = 0;
        for(int i=0; i < amount; i++) {
                expec_val += values1[i] * values2[i] * values3[i] / amount;
        }
        return expec_val;
}

float Expected(float* values1, float* values2, float* values3, float* values4, int amount)
{
        float expec_val = 0;
        for(int i=0; i < amount; i++) {
                expec_val += values1[i] * values2[i] * values3[i] * values4[i] / amount;
        }
        return expec_val;
}

double Expected(double* values1, double* values2, double* values3, double* values4, int amount)
{
        double expec_val = 0;
        for(int i=0; i < amount; i++) {
                expec_val += values1[i] * values2[i] * values3[i] * values4[i] / amount;
        }
        return expec_val;
}

float Expected(float* v1, float* v2, float* v3, float* v4, float* v5, int amount)
{
        float expec_val = 0;
        for(int i=0; i < amount; i++) {
                expec_val += v1[i] * v2[i] * v3[i] * v4[i] * v5[i] / amount;
        }
        return expec_val;
}

double Expected(double* v1, double* v2, double* v3, double* v4, double* v5, int amount)
{
        double expec_val = 0;
        for(int i=0; i < amount; i++) {
                expec_val += v1[i] * v2[i] * v3[i] * v4[i] * v5[i] / amount;
        }
        return expec_val;
}

float Expected(float* v1, float* v2, float* v3, float* v4, float* v5, float*v6, int amount)
{
        float expec_val = 0;
        for(int i=0; i < amount; i++) {
                expec_val += v1[i] * v2[i] * v3[i] * v4[i] * v5[i] * v6[i] / amount;
        }
        return expec_val;
}

double Expected(double* v1, double* v2, double* v3, double* v4, double* v5, double*v6, int amount)
{
        double expec_val = 0;
        for(int i=0; i < amount; i++) {
                expec_val += v1[i] * v2[i] * v3[i] * v4[i] * v5[i] * v6[i] / amount;
        }
        return expec_val;
}

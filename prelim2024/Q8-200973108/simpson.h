#ifndef __SIMPSON_H__
#define __SIMPSON_H__

#include <cmath>

double h(double x);

/**
 ****************************************************************
 * Calculate definite integral using Simpson's rule.
 *
 * This function approximates the definite integral of a given function
 * over a specified interval using Simpson's rule. Simpson's rule divides
 * the interval into subintervals and uses quadratic approximations to
 * estimate the integral.
 *
 * @param n         The number of subintervals. Should be an even number for Simpson's rule.
 * @param l_lim     The lower limit of the integration interval.
 * @param u_lim     The upper limit of the integration interval.
 * @param threads   The number of OpenMP threads to use for parallelization.
 *
 * @return          The estimated definite integral value.
 **************************************************************** 
 */
double simpson(int n, double l_lim, double u_lim, int threads) {
    double dx = (u_lim - l_lim) / n;
    double result = h(l_lim) + h(u_lim);

    #pragma omp parallel for num_threads(threads) reduction(+:result)
    for (int i = 1; i < n; i++) {
        double x = l_lim + i * dx;
        result += (i % 2 == 0) ? 2 * h(x) : 4 * h(x);
    }

    return dx * result / 3.0;
}

#endif
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <tuple>
#include "gauss_elimination.h"

using namespace std;

const double L = 5.0; // Length of the domain
const int N = 256;    // Number of grid points
const double dx = L / N; // Mesh size

// Function to evaluate exact solution
double T_exact(double x) {
    return 100.0 + pow(x, 3);
}

// Function for the source term
double f(double x) {
    return 0.0; // Placeholder, you can modify this according to your problem
}

// Function to evaluate boundary conditions
double evaluate_boundary(double x) {
    return T_exact(x); // Dirichlet boundary condition, T_exact is used as the boundary value
}

// Function to calculate residual
double calculate_residual(double x, double T) {
    double f_val = f(x);
    double T_xx = (T_exact(x + dx) - 2.0 * T + T_exact(x - dx)) / (dx * dx);
    return f_val - T_xx;
}

// Function to return norms
tuple<double, double, double> get_residual(double x) {
    double T = evaluate_boundary(x);
    double residual = calculate_residual(x, T);
    return make_tuple(residual, T, T_exact(x));
}

int main() {
    // Solving the steady-state linear diffusion problem using Gauss elimination

    // Constructing the coefficient matrix A and the vector b
    double A[MAX_SIZE][MAX_SIZE] = {0};
    double b[MAX_SIZE] = {0};

    for (int i = 1; i < N - 1; ++i) {
        A[i][i - 1] = 1.0 / (dx * dx);
        A[i][i] = -2.0 / (dx * dx);
        A[i][i + 1] = 1.0 / (dx * dx);
        b[i] = f(i * dx); // Evaluate the source term at the grid point
    }

    // Applying boundary conditions
    A[0][0] = 1.0;
    b[0] = evaluate_boundary(0);

    A[N - 1][N - 1] = 1.0;
    b[N - 1] = evaluate_boundary(L);

    // Solving the system using Gauss elimination
    double x[N] = {0};
    gauss_elimination(A, b, x, N);

    // Writing the results to a file
    ofstream outfile("results.txt");
    outfile << fixed << setprecision(6);
    for (int i = 0; i < N; ++i) {
        double T = x[i];
        double x_coord = i * dx;
        outfile << "x = " << setw(8) << x_coord << ", ";
        outfile << "T_approx = " << setw(10) << T << ", ";
        outfile << "T_exact = " << setw(10) << T_exact(x_coord) << endl;
    }
    outfile.close();

    return 0;
}

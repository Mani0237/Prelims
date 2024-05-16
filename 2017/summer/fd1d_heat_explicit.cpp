#include <iostream>

using namespace std;

// Function to compute the Courant-Friedrichs-Loewy coefficient
double fd1d_heat_explicit_cfl(double k, int t_num, double t_min, double t_max, int x_num, double x_min, double x_max) {
    double x_step = (x_max - x_min) / (x_num - 1);
    double t_step = (t_max - t_min) / (t_num - 1);
    double cfl = k * t_step / (x_step * x_step);

    if (cfl >= 0.5) {
        cerr << "fd1d_heat_explicit_cfl - Fatal error!" << endl;
        cerr << "CFL condition failed." << endl;
        cerr << "0.5 <= K * dT / dX / dX = " << cfl << endl;
        exit(EXIT_FAILURE);
    }

    return cfl;
}

// Function to solve the 1D heat equation using finite difference method
void fd1d_heat_explicit(int x_num, double x_min, double x_max, double t, double dt, double cfl, double rhs[],
                            void (*bc)(int, double, double, double*), double h[]) {
    double h_new[x_num];
    double f[x_num];
    for (int i = 0; i < x_num; ++i) {
        f[i] = rhs[i];
    }

    for (int c = 1; c < x_num - 1; c++) {
        int l = c - 1;
        int r = c + 1;
        h_new[c] = h[c] + cfl * (h[l] - 2.0 * h[c] + h[r]) + dt * f[c];
    }

    bc(x_num, x_min, x_max, h_new);

    for (int i = 0; i < x_num; ++i) {
        h[i] = h_new[i];
    }
}

// Function to evaluate the boundary conditions for problem 1
void bc_test01(int x_num, double x_min, double x_max, double h[]) {
    h[0] = 90.0;
    h[x_num - 1] = 70.0;
}

// Function to evaluate the initial condition for problem 1
void ic_test01(int x_num, double x_min, double x_max, double h[]) {
    for (int i = 0; i < x_num; ++i) {
        h[i] = 50.0;
    }
}

// Function to evaluate the right-hand side for problem 1
void rhs_test01(int x_num, double x_min, double x_max, double t, double rhs[]) {
    for (int i = 0; i < x_num; ++i) {
        rhs[i] = 0.0;
    }
}

int main() {
    // Parameters for problem 1
    double k = 0.002;
    int x_num = 21;
    double x_min = 0.0, x_max = 1.0;
    int t_num = 201;
    double t_min = 0.0, t_max = 80.0;

    // Initialize t vector
    double t_step = (t_max - t_min) / (t_num - 1);

    // Get the CFL coefficient
    double cfl = fd1d_heat_explicit_cfl(k, t_num, t_min, t_max, x_num, x_min, x_max);

    cout << "CFL coefficient: " << cfl << endl;

    // Initialize h array
    double h[x_num];
    ic_test01(x_num, x_min, x_max, h);

    // Solve the 1D heat equation
    double rhs[x_num];
    for (int j = 1; j < t_num; ++j) {
        rhs_test01(x_num, x_min, x_max, t_min + (j - 1) * t_step, rhs);
        fd1d_heat_explicit(x_num, x_min, x_max, t_min + (j - 1) * t_step, t_step, cfl, rhs, bc_test01, h);
    }

    // Output the result
    cout << "Final temperature profile:" << endl;
    for (int i = 0; i < x_num; ++i) {
        cout << h[i] << " ";
    }
    cout << endl;

    return 0;
}

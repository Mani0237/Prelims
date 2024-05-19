#include <iostream>
#include <cmath>
#include <iomanip>
#include <numeric>

// Constants
const int num_N_values = 6;
const int N_values[num_N_values] = {8, 16, 32, 64, 128, 256};
const double left_x = 0.0;
const double right_x = 5.0;

// Function prototypes
double analytical(double x);
double exact_source(double x);
void FD(int n, double* x, double w, double* initU, double* boundU, double* solU, double* residual, int& k);
double frobNorm(double* A, int size);
double LinfNorm(double* vec, int size);

int main() {
    for (int n_idx = 0; n_idx < num_N_values; ++n_idx) {
        int N = N_values[n_idx];
        int num_zones = N + 1;
        double dx = (right_x - left_x) / (num_zones - 1);

        // Generate mesh points
        double x[num_zones];
        for (int i = 0; i < num_zones; ++i) {
            x[i] = i * dx;
        }

        double initU[num_zones] = {0.0};
        double boundU[num_zones] = {0.0};
        double ua = analytical(left_x);
        double ub = analytical(right_x);
        boundU[0] = ua;
        boundU[num_zones - 1] = ub;

        // Get approximated solution using finite difference method
        double sol[num_zones];
        double residual[num_zones];
        int k;
        double w = 0.5;
        FD(N, x, w, initU, boundU, sol, residual, k);

        // Calculate norms
        double L2 = frobNorm(residual, num_zones);
        double Linf = LinfNorm(residual, num_zones);

        std::cout << std::fixed << std::setprecision(10);
        std::cout << "Results for N = " << N << ":" << std::endl;
        std::cout << "L1 norm: " << std::accumulate(residual, residual + num_zones, 0.0) / N << std::endl;
        std::cout << "L2 norm: " << L2 / N << std::endl;
        std::cout << "Linf norm: " << Linf << std::endl;
        std::cout << std::endl;
    }

    return 0;
}

double analytical(double x) {
    return 100.0 + std::pow(x, 3);
}

double exact_source(double x) {
    return 6 * x;
}

void FD(int n, double* x, double w, double* initU, double* boundU, double* solU, double* residual, int& k) {
    int num_zones = n + 1;
    double h = (x[num_zones - 1] - x[0]) / n;
    double tol = 1.e-5;

    double solUnew[num_zones] = {0.0};
    double solUold[num_zones] = {0.0};

    for (int i = 0; i < num_zones; ++i) {
        if (i == 0) {
            solU[i] = boundU[i];
        } else if (i == num_zones - 1) {
            solU[i] = boundU[i];
        } else {
            solU[i] = initU[i];
        }
    }

    k = 1;
    for (int i = 0; i < num_zones; ++i) {
        solUnew[i] = solU[i];
    }

    while (k < 50) {
        k++;
        for (int i = 1; i < num_zones - 1; ++i) {
            double fval = exact_source(x[i]);
            solUnew[i] = w * (solUold[i - 1] + solUold[i + 1] + (h * h) * fval) / 2.0 + (1 - w) * solUold[i];
        }

        for (int i = 0; i < num_zones; ++i) {
            residual[i] = solUnew[i] - solUold[i];
        }

        for (int i = 0; i < num_zones; ++i) {
            solUold[i] = solUnew[i];
        }
    }

    for (int i = 0; i < num_zones; ++i) {
        solU[i] = solUnew[i];
    }
}

double frobNorm(double* A, int size) {
    double norm = 0.0;
    for (int i = 0; i < size; ++i) {
        norm += A[i] * A[i];
    }
    return std::sqrt(norm);
}

double LinfNorm(double* vec, int size) {
    double maxVal = std::abs(vec[0]);
    for (int i = 1; i < size; ++i) {
        if (std::abs(vec[i]) > maxVal) {
            maxVal = std::abs(vec[i]);
        }
    }
    return maxVal;
}

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <numeric>

// Constants
const int N = 8;
const int num_zones = N + 1;
const double left_x = 0.0;
const double right_x = 5.0;
const double dx = (right_x - left_x) / (num_zones - 1);

// Function prototypes
double analytical(double x);
double exact_source(double x);
void createTridiagonalMatrix(std::vector<std::vector<double>>& matrix);
void matrixVecMul(const std::vector<std::vector<double>>& matrix, const std::vector<double>& vector, std::vector<double>& mul);
double frobNorm(const std::vector<double>& A);
double LinfNorm(const std::vector<double>& vec);
void FD(int n, const std::vector<double>& x, double w, const std::vector<double>& initU, const std::vector<double>& boundU, std::vector<double>& solU, std::vector<double>& residual, int& k);

int main() {
    // Generate mesh points
    std::vector<double> x(num_zones);
    for (int i = 0; i < num_zones; ++i) {
        x[i] = i * dx;
    }

    // Get the analytical solution
    std::vector<double> u(num_zones);
    for (int i = 0; i < num_zones; ++i) {
        u[i] = analytical(x[i]);
    }

    std::vector<double> initU(num_zones, 0.0);
    std::vector<double> boundU(num_zones, 0.0);
    double ua = analytical(left_x);
    double ub = analytical(right_x);
    boundU[0] = ua;
    boundU[num_zones - 1] = ub;

    // Get approximated solution using finite difference method
    std::vector<double> sol(num_zones);
    std::vector<double> residual(num_zones);
    int k;
    double w = 0.5;
    FD(N, x, w, initU, boundU, sol, residual, k);

    // Calculate norms
    double L2 = frobNorm(residual);
    double Linf = LinfNorm(residual);

    std::cout << std::fixed << std::setprecision(10);
    std::cout << "L1 norm for " << N << " meshes is " << std::accumulate(residual.begin(), residual.end(), 0.0) / N << std::endl;
    std::cout << "L2 norm for " << N << " meshes is " << L2 / N << std::endl;
    std::cout << "Linf norm for " << N << " meshes is " << Linf << std::endl;

    return 0;
}

double analytical(double x) {
    return 100.0 + std::pow(x, 3);
}

double exact_source(double x) {
    return 6 * x;
}

double frobNorm(const std::vector<double>& A) {
    double norm = 0.0;
    for (size_t i = 0; i < A.size(); ++i) {
        norm += A[i] * A[i];
    }
    return std::sqrt(norm);
}

double LinfNorm(const std::vector<double>& vec) {
    double maxVal = std::abs(vec[0]);
    for (size_t i = 1; i < vec.size(); ++i) {
        if (std::abs(vec[i]) > maxVal) {
            maxVal = std::abs(vec[i]);
        }
    }
    return maxVal;
}

void FD(int n, const std::vector<double>& x, double w, const std::vector<double>& initU, const std::vector<double>& boundU, std::vector<double>& solU, std::vector<double>& residual, int& k) {
    int num_zones = n + 1;
    double h = 1.0 / n;
    double tol = 1.e-5;

    std::vector<double> solUnew(num_zones, 0.0);
    std::vector<double> solUold = solU;

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
    solUnew = solU;

    while (k < 50) {
        k++;
        for (int i = 1; i < num_zones - 1; ++i) {
            double fval = exact_source(x[i]);
            solUnew[i] = w * (solUold[i - 1] + solUold[i + 1] + (h * h) * fval) / 2.0 + (1 - w) * solUold[i];
        }

        for (int i = 0; i < num_zones; ++i) {
            residual[i] = solUnew[i] - solUold[i];
        }

        solUold = solUnew;
    }

    solU = solUnew;
}

#include <iostream>
#include <cmath>

// Function to approximate: f(x) = (tanh(10x - 5) - tanh(10x + 5))/2
double f(double x) {
    return (std::tanh(10 * x - 5) - std::tanh(10 * x + 5)) / 2;
}

// Calculate Legendre polynomial values for a given x up to order n
void legendre(double x, double values[], int n) {
    values[0] = 1.0;

    if (n > 0)
        values[1] = x;

    for (int i = 2; i <= n; i++) {
        values[i] = ((2 * i - 1) * x * values[i - 1] - (i - 1) * values[i - 2]) / i;
    }
}

// Numerical quadrature using composite midpoint rule
double numericalQuadrature(double (*f)(double), double a, double b, int N) {
    double h = (b - a) / N;
    double sum = 0.0;

    for (int i = 0; i < N; i++) {
        double x = a + (i + 0.5) * h;
        sum += f(x);
    }

    return h * sum;
}

int main() {
    const int n = 4; // Number of Legendre polynomials
    const int N = 1000; // Number of subintervals for numerical quadrature
    double coefficients[n + 1] = {0}; // Coefficients c0, c1, ..., cn
    double legendreValues[n + 1]; // Array to store Legendre polynomial values

    for (int i = 0; i <= n; i++) {
        double (*integrand)(double) = [&](double x) {
            legendre(x, legendreValues, i);
            return f(x) * legendreValues[i];
        };

        coefficients[i] = numericalQuadrature(integrand, -1, 1, N);
    }

    // Print the coefficients
    for (int i = 0; i <= n; i++) {
        std::cout << "c" << i << " = " << coefficients[i] << std::endl;
    }

    return 0;
}

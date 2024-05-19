#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

// Constants
const double a = 0.0;  // left boundary
const double b = 1.0;  // right boundary
const double ua = 90.0;  // boundary condition at a
const double ub = 70.0;  // boundary condition at b
const double u0 = 50.0;  // initial condition
const double k = 0.002;  // thermal diffusivity
const double ti = 0.0;   // initial time
const double tf = 80.0;  // final time

// Function to evaluate the initial condition
double initial_condition(double x) {
    return u0;
}

// Function to evaluate the boundary condition at a
double boundary_condition_a(double t) {
    return ua;
}

// Function to evaluate the boundary condition at b
double boundary_condition_b(double t) {
    return ub;
}

// Function to evaluate the source term
double source_term(double x, double t) {
    return 0.0;
}

// Function to calculate L2 norm
double calculate_L2_norm(const std::vector<double>& u, double dx) {
    double norm = 0.0;
    for (double val : u) {
        norm += val * val * dx;
    }
    return std::sqrt(norm);
}

int main() {
    // Mesh parameters
    std::vector<int> num_zones = {8, 16, 32, 64, 128, 256};
    
    // Loop over different mesh sizes
    for (int N : num_zones) {
        double dx = (b - a) / N;  // mesh spacing
        double dt = 0.5 * dx * dx / k;  // time step for stability

        // Initialize solution vector
        std::vector<double> u(N + 1);
        
        // Apply initial condition
        for (int i = 0; i <= N; ++i) {
            double x = a + i * dx;
            u[i] = initial_condition(x);
        }

        // Time stepping loop
        double t = ti;
        while (t < tf) {
            // Apply boundary conditions
            u[0] = boundary_condition_a(t);
            u[N] = boundary_condition_b(t);

            // Update interior points using explicit finite difference scheme
            for (int i = 1; i < N; ++i) {
                u[i] += k * dt / (dx * dx) * (u[i - 1] - 2 * u[i] + u[i + 1]) + dt * source_term(a + i * dx, t);
            }

            t += dt;  // increment time
        }

        // Output solution to a file
        std::ofstream outfile("solution_N" + std::to_string(N) + ".txt");
        for (int i = 0; i <= N; ++i) {
            outfile << (a + i * dx) << " " << u[i] << std::endl;
        }
        outfile.close();

        // Calculate L2 norm
        double L2_norm = calculate_L2_norm(u, dx);

        // Output norms
        std::cout << "Mesh size: " << N << std::endl;
        std::cout << "L2 Norm: " << L2_norm/N << std::endl;
        std::cout << std::endl;
    }

    return 0;
}

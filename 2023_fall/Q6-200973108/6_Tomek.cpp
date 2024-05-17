#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>

using namespace std;

// Function to initialize the domain with initial conditions
void initialize_domain(double *domain, int size, double ua, double ub, double u0) {
    domain[0] = ua; // Left boundary condition
    domain[size - 1] = ub; // Right boundary condition
    for (int i = 1; i < size - 1; ++i) {
        domain[i] = u0; // Initial condition
    }
}

// Function to solve the heat equation using explicit finite differences
void solve_heat_equation(double *domain, double dt, double dx, double k, int size, int num_steps) {
    double *next_domain = new double[size];

    // Loop over time steps
    for (int step = 0; step < num_steps; ++step) {
        // Apply explicit finite difference scheme to update domain values
        for (int i = 1; i < size - 1; ++i) {
            double diffusion_term = k * (domain[i + 1] - 2 * domain[i] + domain[i - 1]) / (dx * dx);
            next_domain[i] = domain[i] + dt * diffusion_term;
        }

        // Update domain for the next time step
        for (int i = 1; i < size - 1; ++i) {
            domain[i] = next_domain[i];
        }
    }

    delete[] next_domain;
}

// Function to calculate the solution residuals
void calculate_residuals(double *residuals, const double *domain, int size, double dx) {
    // Calculate residuals based on the difference from the exact solution (if available)
    // Here, we assume the exact solution is unknown, so we calculate residuals based on the difference from the initial condition
    for (int i = 0; i < size; ++i) {
        residuals[i] = abs(domain[i] - domain[0]); // Residual = |computed solution - initial condition|
    }
}

// Function to calculate the L1 norm of the residuals
double calculate_l1_norm(const double *residuals, int size, double dx) {
    double norm = 0.0;
    for (int i = 0; i < size; ++i) {
        norm += residuals[i] * dx;
    }
    return norm;
}

// Function to calculate the L2 norm of the residuals
double calculate_l2_norm(const double *residuals, int size, double dx) {
    double norm = 0.0;
    for (int i = 0; i < size; ++i) {
        norm += residuals[i] * residuals[i] * dx;
    }
    return sqrt(norm);
}

// Function to calculate the L-infinity norm of the residuals
double calculate_lInfinity_norm(const double *residuals, int size) {
    double norm = 0.0;
    for (int i = 0; i < size; ++i) {
        double abs_value = abs(residuals[i]);
        if (abs_value > norm) {
            norm = abs_value;
        }
    }
    return norm;
}

int main() {
    // Define problem parameters
    const double ua = 90.0; // Left boundary condition
    const double ub = 70.0; // Right boundary condition
    const double u0 = 50.0; // Initial condition
    const double k = 0.002; // Thermal diffusivity
    const double a = 0.0; // Left end of the domain
    const double b = 1.0; // Right end of the domain
    const double ti = 0.0; // Initial time
    const double tf = 80.0; // Final time
    const int num_zones[] = {8, 16, 32, 64, 128, 256}; // Number of zones for each mesh resolution

    // Output table header
    cout << "----------------------------------------------------------------------------------------" << endl;
    cout << setw(10) << "Mesh Size" << setw(15) << "L1 Norm" << setw(15) << "L2 Norm" << setw(20) << "L-infinity Norm" << setw(15) << "Order (L2)" << endl;
    cout << "----------------------------------------------------------------------------------------" << endl;

    // Store norms and convergence order from the previous iteration
    double l2_norm_prev = 0.0;
    int num_zones_prev = 0;

    // Loop over different mesh sizes
    for (int num_zone : num_zones) {
        double dx = (b - a) / num_zone; // Mesh size
        double dt = 0.5 * dx * dx / k; // Time step size (chosen for stability)
        int num_steps = static_cast<int>((tf - ti) / dt); // Number of time steps

        // Allocate memory for the domain and residuals
        double *domain = new double[num_zone];
        double *residuals = new double[num_zone];

        // Initialize domain with initial conditions
        initialize_domain(domain, num_zone, ua, ub, u0);

        // Solve the heat equation
        solve_heat_equation(domain, dt, dx, k, num_zone, num_steps);

        // Calculate residuals
        calculate_residuals(residuals, domain, num_zone, dx);

        // Calculate L1, L2, and L-infinity norms of the residuals
        double l1_norm = calculate_l1_norm(residuals, num_zone, dx);
        double l2_norm = calculate_l2_norm(residuals, num_zone, dx);
        double linf_norm = calculate_lInfinity_norm(residuals, num_zone);

        // Output norms
        cout << fixed << setw(10) << num_zone << setw(15) << setprecision(4) << l1_norm << setw(15) << l2_norm << setw(20) << linf_norm;

        // Calculate and output convergence order (L2)
        if (num_zone != num_zones[0]) {
            double order_l2 = log(l2_norm / l2_norm_prev) / log(num_zones_prev / static_cast<double>(num_zone));
            cout << setw(15) << setprecision(4) << order_l2 << endl;
        } else {
            cout << setw(15) << "N/A" << endl;
        }

        // Store norms for convergence order calculation in the next iteration
        l2_norm_prev = l2_norm;
        num_zones_prev = num_zone;

        // Free memory
        delete[] domain;
        delete[] residuals;
    }

    return 0;
}

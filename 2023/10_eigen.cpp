#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <cmath>

// Define the analytic solution
double T_a(double x) {
    return 100 + pow(x, 3);
}

// Define the source term
double source_term(double x) {
    return 6 * x;
}

int main() {
    // Domain definition
    double L = 5.0;
    double T_0 = 100.0;
    double T_L = 225.0;
    
    // Number of intervals
    int N = 10;  // You can change this for convergence study
    double dx = L / N;
    
    // Discretize the domain
    std::vector<double> x(N + 1);
    for (int i = 0; i <= N; ++i) {
        x[i] = i * dx;
    }
    
    // Initialize the matrix A and the vector b
    Eigen::MatrixXd A = Eigen::MatrixXd::Zero(N + 1, N + 1);
    Eigen::VectorXd b = Eigen::VectorXd::Zero(N + 1);
    
    // Apply boundary conditions
    A(0, 0) = 1;
    b(0) = T_0;
    A(N, N) = 1;
    b(N) = T_L;
    
    // Fill the matrix A and vector b for the interior points
    for (int i = 1; i < N; ++i) {
        A(i, i - 1) = 1.0 / (dx * dx);
        A(i, i) = -2.0 / (dx * dx);
        A(i, i + 1) = 1.0 / (dx * dx);
        b[i] = source_term(x[i]);
    }
    
    // Solve the system
    Eigen::VectorXd T_n = A.colPivHouseholderQr().solve(b);
    
    // Calculate the exact solution and error
    Eigen::VectorXd T_exact(N + 1);
    Eigen::VectorXd error(N + 1);
    for (int i = 0; i <= N; ++i) {
        T_exact[i] = T_a(x[i]);
        error[i] = std::abs(T_n[i] - T_exact[i]);
    }
    
    // Output the results
    std::cout << "x\tNumerical\tAnalytic\tError\n";
    for (int i = 0; i <= N; ++i) {
        std::cout << x[i] << "\t" << T_n[i] << "\t" << T_exact[i] << "\t" << error[i] << "\n";
    }
    
    // Calculate the L2 error norm
    double L2_error = std::sqrt(error.squaredNorm() / (N + 1));
    std::cout << "L2 Error: " << L2_error << "\n";
    
    return 0;
}

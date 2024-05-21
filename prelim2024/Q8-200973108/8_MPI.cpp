#include <iostream>
#include <cmath>
#include <mpi.h>
#include <iomanip>
#include <vector>

using namespace std;

// Function to be integrated
double h(double x) {
    return exp(x);
}

// Simpson's rule to be used by MPI processes
double integrate_simpsons(double x0, double x2, double dx) {
    return (dx / 3.0) * (h(x0) + 4 * h(x0 + dx / 2.0) + h(x2));
}

int main(int argc, char** argv) {
    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Get rank and size of the MPI processes
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int Np = size; // Number of processes
    const int N = 10;    // Number of subintervals per process

    double a = 0.0;    // Lower limit of integration
    double b = 1.0;    // Upper limit of integration

    double dx = (b - a) / (2 * Np * N); // Width of each subinterval

    double local_sum = 0.0;
    double x0 = a + rank * N * dx * 2; // Starting point for this process
    double x2 = x0 + 2 * N * dx;       // End point for this process

    // Calculate local integral using Simpson's rule
    for (int i = 0; i < N; i++) {
        local_sum += integrate_simpsons(x0, x0 + dx, dx);
        x0 += 2 * dx; // Move to the next subinterval
    }

    // exchanging function values
    if (rank < size - 1) {
        double next_h2 = h(x2 + dx);
        MPI_Send(&next_h2, 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
    }
    if (rank > 0) {
        MPI_Recv(&x2, 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Gather all local results on process 0
    vector<double> all_local_sums(size);
    MPI_Gather(&local_sum, 1, MPI_DOUBLE, all_local_sums.data(), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Output results
    if (rank == 0) {
        cout << "-------------------------" << endl;
        cout << setw(10) << left << "Process" << setw(15) << "Area" << endl;
        cout << "-------------------------" << endl;
        double total_sum = 0.0;
        for (int i = 0; i < size; ++i) {
            cout << setw(10) << left << i << setw(15) << all_local_sums[i] << endl;
            total_sum += all_local_sums[i];
        }
        cout << "-------------------------" << endl;
        cout << "Full approximation: " << total_sum << endl;
    }

    // Finalize MPI
    MPI_Finalize();
    return 0;
}

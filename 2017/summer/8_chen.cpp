#include <iostream>
#include <cmath>
#include <mpi.h>

double f(double x) {
    return x + 5 * x * x - 0.5 * x * x * x;
}

double trapezoid(double local_a, double local_b, int local_n, double h) {
    double sum = (f(local_a) + f(local_b)) / 2.0;
    for (int i = 1; i < local_n; ++i) {
        double x = local_a + i * h;
        sum += f(x);
    }
    return h * sum;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double a = 0.0;
    double b = 10.0;
    int Np = size;  // Number of processes

    int local_n = (b - a) / Np;  // Number of subintervals per process
    double h = (b - a) / (Np * local_n);  // Width of each subinterval

    double local_a = a + rank * local_n * h;
    double local_b = local_a + local_n * h;

    double local_area = trapezoid(local_a, local_b, local_n, h);
    double total_area;

    MPI_Reduce(&local_area, &total_area, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Integral: " << total_area << std::endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);  // Wait for all processes to reach this point

    // Print the areas calculated by each process
    std::cout << "Process " << rank << ": Area = " << local_area << std::endl;

    MPI_Finalize();

    return 0;
}

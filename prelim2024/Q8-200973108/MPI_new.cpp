#include <mpi.h>
#include <iostream>
#include <cmath>
#include <iomanip>  // For formatted output

#define H(x) (std::exp(x)) // Define the function h(x) = e^x

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int Np = size;  // Number of processes
    double a = 0.0, b = 1.0;  // Interval [0,1]
    double h = (b - a) / (2 * Np);  // Width of each subinterval
    double x2i = a + 2 * rank * h;  // x2i for the current process
    double x2i1 = x2i + h;  // x2i+1
    double x2i2 = x2i + 2 * h;  // x2i+2

    double h_x2i = H(x2i);  // h(x2i)
    double h_x2i1 = H(x2i1);  // h(x2i+1)
    double h_x2i2 = 0.0;

    if (rank < Np - 1) {
        MPI_Send(&h_x2i1, 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&h_x2i2, 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
        h_x2i2 = H(x2i2);  // Last process calculates h(x2Np)
    }

    if (rank > 0) {
        MPI_Recv(&h_x2i, 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(&h_x2i1, 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD);
    }

    double Ai = h * (h_x2i + 4 * h_x2i1 + h_x2i2) / 3.0;  // Calculate Ai

    // Gather all local sums at the root process
    double* all_local_sums = nullptr;
    if (rank == 0) {
        all_local_sums = new double[size];
    }
    MPI_Gather(&Ai, 1, MPI_DOUBLE, all_local_sums, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Output the results in the desired format
        std::cout << "-------------------------" << std::endl;
        std::cout << std::setw(10) << std::left << "Process" << std::setw(15) << "Area" << std::endl;
        std::cout << "-------------------------" << std::endl;
        double total_sum = 0.0;
        for (int i = 0; i < size; ++i) {
            std::cout << std::setw(10) << std::left << i << std::setw(15) << all_local_sums[i] << std::endl;
            total_sum += all_local_sums[i];
        }
        std::cout << "-------------------------" << std::endl;
        std::cout << "Full approximation: " << total_sum << std::endl;

        delete[] all_local_sums; // Clean up memory
    }

    MPI_Finalize();
    return 0;
}

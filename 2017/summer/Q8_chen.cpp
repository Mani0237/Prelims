#include <stdio.h>
#include <iostream>
#include <mpi.h>

using namespace std;

// defining the function whose integral is to be calculated
double f(double x){
    return x + 5 * x * x - 0.5 * x * x * x; 
}

/**
 * -----------------------------------------------------------
 * @brief Function to implement trapezoidal rule
 * 
 * @param x0 - starting point of integration interval
 * @param x1 - ending point of integration interval
 * @param n  - number of subintervals between x0 and x1
 *
 * @return - the value of integration of f between x0 and x1
 * -----------------------------------------------------------
*/
double trapezoidal(double x0, double x1, int n){
    double dx = (x1 - x0) / n;
    double sum = 0.5 * (f(x0) + f(x1));

    for (int i = 1; i < n; i++) {
        double xi = x0 + i * dx;
        sum = sum + f(xi);
    }
    
    return sum * dx;
}

int main(int argc, char *argv[]) {
    
    // initializing MPI and defining MPI parameters
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int Np = size;

    // limits of complete integration
    double xi = 0.0;
    double xf = 10.0;
    int subintervals = 100;
    double dx = (xf - xi) / (Np * subintervals);
    
    // defining limits of local integration for each process
    double local_xi = xi + rank * dx * subintervals;
    double local_xf = local_xi + dx * subintervals;

    // calculting local integral
    double local_integral = trapezoidal(local_xi, local_xf, subintervals);
    double total_integral;

    // using MPI_reduce to sum up all local integrals
    MPI_Reduce(&local_integral, &total_integral, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // printing the results using master
    if (rank == 0) {
        cout << "The total integral is = " << total_integral << endl;
    }

    MPI_Finalize();
    return 0;

}
#include <iostream>
#include <cmath>
#include <iomanip>
#include <random>
#include <chrono>
#include <mpi.h>

using namespace std;

pair<double, double> Calculate_area(double inner_radius, double outer_radius, int num_samples_per_process) {
    random_device rd;
    mt19937 gen(rd());
    int count = 0;

    // Start timing
    double start_time = MPI_Wtime();
    
    for(int i = 0; i < num_samples_per_process; i++) {
        double x = uniform_real_distribution<double>(-outer_radius, outer_radius)(gen);
        double y = uniform_real_distribution<double>(-outer_radius, outer_radius)(gen);
        double distance = sqrt(x*x + y*y);
        if(distance <= outer_radius && distance >= inner_radius) {
            count++;
        }
    }
    
    double area = 4 * (outer_radius * outer_radius) * (double(count) / num_samples_per_process);

    // End timing
    double end_time = MPI_Wtime();
    double duration = end_time - start_time;

    return make_pair(area, duration);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int num_processes, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double r = 10.0;
    double R = 20.0;
    double A_exact = 300.0 * M_PI;
    int N = 10000000;

    int num_samples_per_process = N / num_processes;
    pair<double, double> local_result = Calculate_area(r, R, num_samples_per_process);

    double local_area = local_result.first;
    double local_duration = local_result.second;

    double global_area, global_duration;
    MPI_Reduce(&local_area, &global_area, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_duration, &global_duration, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if(rank == 0) {
        cout << "-------------------------------------------------------------------" << endl;
        cout << left << setw(12) << "Threads"
             << left << setw(20) << "Calculated Area"
             << left << setw(20) << "Error"
             << left << setw(20) << "Duration (s)" << endl;
        cout << "-------------------------------------------------------------------" << endl;

        cout << left << setw(12) << num_processes
             << left << setw(20) << global_area
             << left << setw(20) << abs(A_exact - global_area)
             << left << setw(20) << global_duration << endl;

        cout << "-------------------------------------------------------------------" << endl;
    }

    MPI_Finalize();
    return 0;
}

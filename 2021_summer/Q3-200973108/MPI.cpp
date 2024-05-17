#include <iostream>
#include <chrono>
#include <iomanip>
#include <mpi.h>

#define n 1000000000

using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    unsigned char *s = new unsigned char[n];

    // Generate random data
    for (int i = 0; i < n; i++) {
        s[i] = i * i % 256;
    }

    int *num = new int[256]();
    int *local_num = new int[256]();

    // Distribute workload among processes
    int local_n = n / size;
    MPI_Scatter(s, local_n, MPI_UNSIGNED_CHAR, s, local_n, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
   
    auto start = chrono::steady_clock::now();
    // Count occurrences locally
    for (int i = 0; i < local_n; i++) {
        local_num[s[i]]++;
    }
    auto end = chrono::steady_clock::now();
    
    // Reduce local counts to global counts
    MPI_Reduce(local_num, num, 256, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Print results in root process
    if (rank == 0) {
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "Execution time: " << duration << " milliseconds" << endl;
    }

    delete[] s;
    delete[] num;
    delete[] local_num;

    MPI_Finalize();
    return 0;
}


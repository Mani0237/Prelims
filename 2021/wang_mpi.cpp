#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <iostream>

using namespace std;

#define  n 1000000000
unsigned char s[n];

int main(int argc, char *argv[]){

    int rank, size;
    int i;
    double start_time, end_time;

    //initialize MPI
    MPI_Init(&argc, &argv);

    // Get the rank and size
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);    

    // Initialize array s
    for(i = 0; i < n; i++){
        s[i] = i * i % 256;
    }

    // Divide the work among ranks
    int chunk_size = n/size;
    int start = rank * chunk_size;
    int end = start + chunk_size;

    start_time = MPI_Wtime();
    int num_local[256] = {0};
        
    for(i = start; i < end; i++){
        num_local[s[i]]++;
    }

    int num_global[256] = {0};
    MPI_Reduce(num_local, num_global, 256, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    end_time = MPI_Wtime();

    // Print the character frequency on rank 0
    if(rank == 0){
        for(i = 0; i < 256; i++){
            //printf("The character %d frequency is %lf\n", i, num_global[i] / (double)n);
        }
        printf("Time = %f sec\n", end_time - start_time);
    }    

    // Finalize MPI
    MPI_Finalize();

    return 0;
}
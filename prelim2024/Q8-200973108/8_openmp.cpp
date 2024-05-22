#include <iostream>
#include <omp.h>
#include <cmath>
#include <vector>
#include <iomanip>
#include <chrono>
#include "simpson.h"

using namespace std;
using namespace std::chrono;


// Function to be integrated
double h(double x) {
    return exp(x);
}


int main() {

    const int n = 1000;    // Number of intervals
    double a = 0.0;        // lower limit of integration
    double b = 1.0;        // upper limit of integration
    const double I_exact = 1.718281828459045;                  // exact value from integral calculator
    vector<int> thread_counts = {1, 2, 4, 6, 8, 10, 16, 28};   // thread count

    // formating the output
    cout << "------------------------------------------------------------" << endl;
    cout << setw(10) << left << "Threads"
         << setw(10) << left << "I"
         << setw(18) << left << "Error" 
         << setw(20) << "Time(microseconds)"
         << endl;
    cout << "------------------------------------------------------------" << endl;

    // Loop over different threads
    for (int num_threads : thread_counts) {

        // starting the timer
        auto start_time = high_resolution_clock::now();

        // caling function to calculate integral
        double I = simpson(n, a, b, num_threads);
        
        // stoping the timer
        auto end_time = high_resolution_clock::now();
        double elapsed_time = duration_cast<microseconds>(end_time - start_time).count();
        
        // calculting error
        double error = abs(I - I_exact);
        
        // printing the results for each thread
        cout << setw(10) << num_threads 
             << setw(10) << I 
             << setw(18) << error 
             << setw(20) << elapsed_time 
             << endl;
    }
    cout << "------------------------------------------------------------" << endl;

    return 0;
}

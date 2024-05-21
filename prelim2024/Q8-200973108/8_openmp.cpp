#include <iostream>
#include <omp.h>
#include <cmath>
#include <vector>
#include <iomanip>
#include <chrono>
#include "simpson.h"

using namespace std;
using namespace std::chrono;

double h(double x) {
    return exp(x);
}


int main() {

    const int n = 1000000; 
    double a = 0.0;
    double b = 1.0;
    const double I_exact = 1.718281828459045;
    vector<int> thread_counts = {1, 2, 4, 6, 8, 10, 16, 28}; 

    cout << "------------------------------------------------------------" << endl;
    cout << setw(10) << left << "Threads"
         << setw(10) << left << "I"
         << setw(18) << left << "Error" 
         << setw(20) << "Time(microseconds)"
         << endl;
    cout << "------------------------------------------------------------" << endl;

    for (int num_threads : thread_counts) {
        auto start_time = high_resolution_clock::now();
        double I = simpson(n, a, b, num_threads);
        auto end_time = high_resolution_clock::now();
        double elapsed_time = duration_cast<microseconds>(end_time - start_time).count();
        double error = abs(I - I_exact);
        cout << setw(10) << num_threads 
             << setw(10) << I 
             << setw(18) << error 
             << setw(20) << elapsed_time 
             << endl;
    }
    cout << "------------------------------------------------------------" << endl;

    return 0;
}

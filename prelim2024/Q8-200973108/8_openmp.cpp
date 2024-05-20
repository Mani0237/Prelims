#include <iostream>
#include <omp.h>
#include <cmath>
#include <vector>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

double h(double x) {
    return exp(x);
}

double integrate_simpsons(int n, int num_threads) {
    double a = 0.0;
    double b = 1.0;
    double dx = (b - a) / n;
    double sum = 0.0;

    #pragma omp parallel for reduction(+:sum) num_threads(num_threads)
    for (int i = 0; i < n; i++) {
        double x = a + i * dx;
        double term = h(x);
        if (i > 0 && i < n - 1) {
            if (i % 2 == 0) {
                term *= 2.0;
            } else {
                term *= 4.0;
            }
        }
        sum += term;
    }

    sum = (sum + h(a) + h(b)) * dx / 3.0;

    return sum;
}

int main() {
    const int n = 1000000; // Increase the workload size
    const double I_exact = 1.718281828459045;
    vector<int> thread_counts = {1, 2, 4, 6, 8, 10}; 

    cout << "------------------------------------------------------------" << endl;
    cout << setw(10) << left << "Threads"
         << setw(10) << left << "I"
         << setw(18) << left << "Error" 
         << setw(20) << "Time(microseconds)"
         << endl;
    cout << "------------------------------------------------------------" << endl;

    for (int num_threads : thread_counts) {
        auto start_time = high_resolution_clock::now();
        double I = integrate_simpsons(n, num_threads);
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

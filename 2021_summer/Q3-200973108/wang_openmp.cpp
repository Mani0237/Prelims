#include <iostream>
#include <chrono>
#include <iomanip>
#include <omp.h>

#define n 1000000000

using namespace std;

unsigned char s[n];

int main() {
    int num[256] = {0};

    // Initialize s array
    for (int i = 0; i < n; i++) {
        s[i] = i * i % 256;
    }

    // Get number of available threads
    int num_threads = omp_get_max_threads();

    // Run the loop with different number of threads
    for (int num_threads = 1; num_threads <= omp_get_max_threads(); num_threads++) {
        auto start = chrono::steady_clock::now();

        // Parallel loop with reduction for accumulating results
        #pragma omp parallel for num_threads(num_threads) reduction(+:num[:256])
        for (int i = 0; i < n; i++) {
            num[s[i]]++;
        }

        auto end = chrono::steady_clock::now();

        // Calculate execution time
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "Threads: " << num_threads << ", Execution time: " << duration << " milliseconds" << endl;
    }

    return 0;
}


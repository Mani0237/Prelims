#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>
#include <iomanip>
#include <chrono>

using namespace std;

// Function to check if a number is prime
bool isPrime(int num) {
    if (num <= 1) {
        return false;
    }
    for (int i = 2; i <= sqrt(num); ++i) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}

// Function to find the largest prime gap between n and m
int largestPrimeGap(int n, int m) {
    int largestGap = 0;
    int previousPrime = -1;

    // Parallelize the loop using OpenMP
    #pragma omp parallel reduction(max:largestGap)
    {
        int largestGapPrivate = 0;
        int previousPrimePrivate = -1;

        #pragma omp for
        for (int i = n; i <= m; ++i) {
            // Check if current number is prime
            if (isPrime(i)) {
                // If previousPrime is valid, calculate the gap and update largestGapPrivate if necessary
                if (previousPrimePrivate != -1) {
                    int gap = i - previousPrimePrivate;
                    if (gap > largestGapPrivate) {
                        largestGapPrivate = gap;
                    }
                }
                previousPrimePrivate = i;
            }
        }

        #pragma omp critical
        {
            if (largestGapPrivate > largestGap) {
                largestGap = largestGapPrivate;
            }
        }
    }
    
    return largestGap;
}

int main() {
    int n, m;

    cout << "Enter two numbers (n and m): ";
    cin >> n >> m;
    cout << endl;

    cout << "**********************************************************************" << endl;
    cout << "Largest prime number gap calculation between " << n << " and " << m << endl;
    cout << "**********************************************************************" << endl;
    cout << endl;

    int threads[] = {1, 2, 4, 8, 16, 32, 64};

    cout << "------------------------------------" << endl;
    cout << setw(10) << left << "Threads" <<  setw(12) << "Gap" << setw(10) << "Time(s)" << endl;
    cout << "------------------------------------" << endl;    

    for (int num_threads : threads) {
        omp_set_num_threads(num_threads);

        auto start = chrono::system_clock::now();
        
        int largestGap = largestPrimeGap(n, m);
        
        auto end = chrono::system_clock::now();
        auto time_elapsed = chrono::duration_cast<chrono::milliseconds>(end - start); 

        cout << setw(10) << num_threads << setw(12) << largestGap << setw(10) << time_elapsed.count()/1000.0 << endl;
    
    }
    cout << "------------------------------------" << endl;    

    return 0;
}

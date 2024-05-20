#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>
#include <chrono>
#include <iomanip>

using namespace std;

// Function to generate primes using Sieve of Eratosthenes
void generate_primes(bool* is_prime, int limit) {
    fill(is_prime, is_prime + limit + 1, true);
    is_prime[0] = is_prime[1] = false;

    for (int i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= limit; j += i) {
                is_prime[j] = false;
            }
        }
    }
}

// Function to verify Goldbach's conjecture
bool verify_goldbach(bool* is_prime, int limit, omp_sched_t schedule_type, int chunk_size) {
    bool valid = true;

    omp_set_schedule(schedule_type, chunk_size);

    #pragma omp parallel for schedule(runtime)
    for (int n = 4; n <= limit; n += 2) {
        bool found = false;
        for (int i = 2; i <= n / 2; ++i) {
            if (is_prime[i] && is_prime[n - i]) {
                found = true;
                break;
            }
        }
        if (!found) {
            #pragma omp critical
            {
                std::cout << "Goldbach's conjecture is wrong for " << n << std::endl;
                valid = false;
            }
        }
    }
    return valid;
}

int main() {
    const int limit = 10000000;
    bool* is_prime = new bool[limit + 1];

    auto start = chrono::high_resolution_clock::now();
    generate_primes(is_prime, limit);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> diff = end - start;
    cout << endl;
    cout << "Time to generate primes: " << diff.count() << " s" << endl;
    cout << endl;

    const int num_threads_list[] = {1, 2, 4, 8, 16, 28};
    const omp_sched_t schedules[] = {omp_sched_static, omp_sched_dynamic, omp_sched_guided};
    const char* schedule_names[] = {"Static", "Dynamic", "Guided"};

    // Print table header
    cout << "============================================================" << endl;
    cout << left << setw(10) << "Threads" 
         << left << setw(25) << "Time (s)" 
         << left << setw(25) << "Goldbach Conjecture" << endl;
    cout << "============================================================" << endl;
    
    for (int s = 0; s < 3; ++s) {
        auto schedule_type = schedules[s];
        auto schedule_name = schedule_names[s];

        cout << "----------------------------------------------------" << endl;
        cout << "                     " << schedule_name << endl;
        cout << "----------------------------------------------------" << endl;
        for (auto num_threads : num_threads_list) {
            omp_set_num_threads(num_threads);

            start = chrono::high_resolution_clock::now();
            bool result = verify_goldbach(is_prime, limit, schedule_type, 100);
            end = chrono::high_resolution_clock::now();
            diff = end - start;

            cout << left << setw(10) << num_threads
                 << left << setw(25) << diff.count()
                 << left << setw(25) << (result ? "Holds" : "False") << endl;
        }
    }
    cout << "============================================================" << endl;

    delete[] is_prime;
    return 0;
}
